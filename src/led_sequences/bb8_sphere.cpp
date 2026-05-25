// Copyright (c) 2026 Douglas Kempthorne (douglas@kempthorne.com)
// SPDX-License-Identifier: GPL-3.0-or-later

#include <Arduino.h>
#include "led_sequences.h"
#include "bb8_sphere.h"

// BB-8 CNC Sphere — 17 LEDs on one continuous strip.
// Clockwise directions are from the EXTERIOR of the sphere.
//
// Strip order:  C6 (6) → C3 (2) → C5 (1) → C1 (4) → C2 (2) → C4 (2)
// Array index:   0–5      6–7      8        9–12     13–14    15–16

// ─────────────────────────────────────────────────────────────────────────────
//  LED Control Matrix
//
//  Columns:  name | blink? | onMs | offMs | jitterMs | startMs | nColors | palette
//
//  Editing guide:
//    • blinkEnabled false  → LED stays on at palette[0], all timing ignored.
//    • jitterMs 0          → perfectly metronomic blink, no randomness.
//    • colorCount > 1      → cycles through palette on each blink cycle.
//    • palette entries beyond colorCount are ignored.
// ─────────────────────────────────────────────────────────────────────────────
BB8LedControl bb8_leds[BB8_LED_COUNT] = {

    // ── CIRCLE 6 — tight trio (C6_1–C6_3) + 2+1 group (C6_4–C6_6) ──────────
    //
    //                                  "name",  blink?, onMs,offMs,jitter, startms, palette
    //  Tight trio: pure white, slightly different on/off to avoid lockstep.
    /* [0]  C6-1  tight trio, first  */ {"C6-1",  true,  750, 750,  0,   0,   1, {BB8_RED}},
    /* [1]  C6-2  tight trio, middle */ {"C6-2",  false,  800, 1200, 0,   0,   1, {BB8_LIGHT_BLUE}},
    /* [2]  C6-3  tight trio, last   */ {"C6-3",  true,  750, 750,  0,   750, 1, {BB8_RED}},
    //
    //  2+1 group: cycles through three blues for a slow colour-shift effect.
    /* [3]  C6-4  2+1 group, first   */ {"C6-4",  true,  250,  250, 100,    0, 3, {BB8_BLUE, BB8_DARK_BLUE, BB8_TEAL_CYAN}},
    /* [4]  C6-5  2+1 group, middle  */ {"C6-5",  false,  500,  500, 0,     0, 1, {BB8_AMBER}},
    /* [5]  C6-6  2+1 group, last    */ {"C6-6",  false,  500,  500, 0,   500, 1, {BB8_RED}},

    // ── CIRCLE 3 — pair, narrow rectangle (C3_7–C3_8) ───────────────────────
    /* [6]  C3-7  pair, first        */ {"C3-7",  false, 1200,  0, 0,   0, 1, {BB8_TEAL_BRIGHT}},
    /* [7]  C3-8  pair, second       */ {"C3-8",  false,  900,  0, 0,   0, 1, {BB8_TEAL}},

    // ── CIRCLE 5 — single, narrow rectangle (C5_9) ───────────────────────────
    /* [8]  C5-9  single             */ {"C5-9",  false,  600, 1400, 600,   0, 1, { BB8_LIGHT_BLUE}},

    // ── CIRCLE 1 — four LEDs, narrow rectangle (C1_10–C1_13) ────────────────
    /* [9]  C1-10 first              */ {"C1-10", true,  30000,   5, 0,   0, 3,   {BB8_DARK_BLUE, BB8_WHITE, BB8_LIGHT_BLUE}},
    /* [10] C1-11 second             */ {"C1-11", true,  20000,   5, 0,   0, 3,   {BB8_WHITE, BB8_TEAL_DARK, BB8_TEAL_CYAN}},
    /* [11] C1-12 third              */ {"C1-12", true,  40000,   5, 0,   0, 3,   {BB8_WHITE, BB8_LIGHT_BLUE, BB8_BLUE}},
    /* [12] C1-13 fourth             */ {"C1-13", true,  60000,   5, 0,   0, 3,   {BB8_TEAL_DARK, BB8_TEAL_CYAN, BB8_WHITE}},

    // ── CIRCLE 2 — two LEDs, square (C2_14–C2_15) ────────────────────────────
    /* [13] C2-14 outer              */ {"C2-14", false, 1500,  500, 400,   0, 1, {BB8_LIGHT_BLUE}},
    /* [14] C2-15 center             */ {"C2-15", true, 1200,  800, 250,   0, 3, {BB8_RED, BB8_ORANGE, BB8_AMBER}},

    // ── CIRCLE 4 — locking ring panel, two LEDs, square (C4_16–C4_17) ────────
    /* [15] C4-16 closest to center  */ {"C4-16", true,  5000, 5000, 0,    0, 1, {BB8_TEAL}},
    /* [16] C4-17 furthest from ctr  */ {"C4-17", true,  5000, 5000, 0, 5000, 1, {BB8_DARK_BLUE}},
};

// ─────────────────────────────────────────────────────────────────────────────
//  initBB8Matrix
//
//  Call once at startup (or whenever you want to reset the animation state).
//  Staggers initial wake times by a prime-number spacing so LEDs don't sync up.
// ─────────────────────────────────────────────────────────────────────────────
void initBB8Matrix() {
    unsigned long now = millis();
    for (uint8_t i = 0; i < BB8_LED_COUNT; i++) {
        BB8LedControl &led = bb8_leds[i];
        led.isOn         = false;
        led.colorIdx     = 0;
        led.nextToggleMs = now + (led.startDelayMs > 0 ? led.startDelayMs : (uint16_t)(i * 137u));
    }
}

// ─────────────────────────────────────────────────────────────────────────────
//  updateBB8Matrix
//
//  Call every loop iteration. Checks each LED's timer, toggles state when due,
//  advances the color palette on the OFF→ON transition, and writes the result
//  to the strip buffer. Calls strip.show() once at the end.
// ─────────────────────────────────────────────────────────────────────────────
bool updateBB8Matrix(Adafruit_NeoPixel &strip) {
    unsigned long now = millis();
    bool anyChange = false;

    for (uint8_t i = 0; i < BB8_LED_COUNT; i++) {
        BB8LedControl &led = bb8_leds[i];

        // ── Blink state machine ──────────────────────────────────────────────
        if (led.blinkEnabled && now >= led.nextToggleMs) {
            led.isOn = !led.isOn;
            anyChange = true;
            if (led.isOn) {
                // OFF→ON: advance to next color, schedule ON duration + jitter
                led.colorIdx     = (led.colorIdx + 1) % led.colorCount;
                led.nextToggleMs = now + led.blinkOnMs
                                       + (led.randomJitterMs ? random(0, led.randomJitterMs) : 0);
            } else {
                // ON→OFF: schedule OFF duration + jitter
                led.nextToggleMs = now + led.blinkOffMs
                                       + (led.randomJitterMs ? random(0, led.randomJitterMs) : 0);
            }
        }

        // ── Write pixel ──────────────────────────────────────────────────────
        if (led.isOn || !led.blinkEnabled) {
            const BB8Color &c = led.palette[led.colorIdx];
            strip.setPixelColor(i, c.r, c.g, c.b);
        } else {
            strip.setPixelColor(i, 0, 0, 0);
        }
    }

    return anyChange;
}

// ─────────────────────────────────────────────────────────────────────────────
//  led_sphere_loop  (diagnostic / colour-test routine — not the live matrix)
// ─────────────────────────────────────────────────────────────────────────────

#define TOTAL_LEDS 17

#ifndef BB8_SPHERE_ONLY
#define BB8_SPHERE_ONLY 0
#endif

bool led_sphere_loop(Adafruit_NeoPixel &s) {
    #if BB8_SPHERE_ONLY == 0
    return false; // disabled outside sphere-only test builds
    #endif

    static unsigned long last_ms = 0;
    static uint8_t phase = 0;
    unsigned long now = millis();

    if (now - last_ms < 2000) return false;
    last_ms = now;

    LedStrip ls(s, false, false, 255, 0, TOTAL_LEDS);
    switch (phase) {
        case 0: solidFill(ls, HEX_red);   Serial.println("LED Test: Red");            break;
        case 1: solidFill(ls, HEX_green); Serial.println("LED Test: Green");          break;
        case 2: solidFill(ls, HEX_blue);  Serial.println("LED Test: Blue");           break;
        case 3: solidFill(ls, HEX_white); Serial.println("LED Test: White (RGB mix)"); break;
    }
    if (++phase > 3) phase = 0;
    return true;
}
