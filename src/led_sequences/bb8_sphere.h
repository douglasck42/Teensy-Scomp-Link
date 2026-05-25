// Copyright (c) 2026 Douglas Kempthorne (douglas@kempthorne.com)
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <Arduino.h>
#include "led_sequences.h"
#include <Adafruit_NeoPixel.h>

// ─────────────────────────────────────────────────────────────────────────────
//  BB8_LED_COUNT is fixed. BB8_MAX_COLORS is the palette slots per LED;
//  increase it here if you ever need more than 4 colors on one LED.
// ─────────────────────────────────────────────────────────────────────────────
#define BB8_LED_COUNT   17
#define BB8_MAX_COLORS   4

// ── Color type ────────────────────────────────────────────────────────────────
struct BB8Color {
    uint8_t r, g, b;
};

// ── Named color palette (BB-8 blue/white theme + utility) ────────────────────
//    Add entries here and reference them by name in the matrix below.
static constexpr BB8Color BB8_OFF        = {  0,   0,   0};
static constexpr BB8Color BB8_WHITE      = {255, 255, 255};
static constexpr BB8Color BB8_WARM_WHITE = {255, 240, 200};
static constexpr BB8Color BB8_LIGHT_BLUE = {100, 180, 255};
static constexpr BB8Color BB8_BLUE       = { 30, 100, 255};
static constexpr BB8Color BB8_DARK_BLUE  = {  0,  30, 120};
static constexpr BB8Color BB8_AMBER      = {255, 140,   0};
static constexpr BB8Color BB8_ORANGE     = {255,  80,   0};
static constexpr BB8Color BB8_RED        = {255,   0,   0};
static constexpr BB8Color BB8_GREEN      = {  0, 255,   0};
static constexpr BB8Color BB8_BLUE_ONLY   = {  0,   0, 255};
static constexpr BB8Color BB8_YELLOW     = {255, 255,   0};
static constexpr BB8Color BB8_TEAL       = {  0, 180, 160};
static constexpr BB8Color BB8_TEAL_BRIGHT= {  0, 200, 180};
static constexpr BB8Color BB8_TEAL_DARK  = {  0, 140, 130};
static constexpr BB8Color BB8_TEAL_CYAN  = {  0, 180, 220};

// ── Per-LED control block ─────────────────────────────────────────────────────
struct BB8LedControl {

    // ── Identity ──────────────────────────────────────────────────────────────
    const char* name;           // label shown in serial debug output

    // ── Blink timing (milliseconds) ───────────────────────────────────────────
    bool     blinkEnabled;      // false = always on at palette[0], no toggling
    uint16_t blinkOnMs;         // how long the LED stays ON per blink cycle
    uint16_t blinkOffMs;        // how long the LED stays OFF per blink cycle

    // ── Randomizer ────────────────────────────────────────────────────────────
    //    Each ON and OFF interval independently adds random(0, randomJitterMs).
    //    Set to 0 to disable jitter and get a perfectly metronomic blink.
    uint16_t randomJitterMs;

    // ── Start delay ───────────────────────────────────────────────────────────
    //    How long to wait (ms) before this LED starts its first blink cycle.
    //    When non-zero, overrides the automatic stagger in initBB8Matrix so the
    //    delay is exact (e.g. two LEDs with matching onMs/offMs but one delayed
    //    by half the period will alternate in perfect opposition).
    //    Set to 0 to use the default auto-stagger.
    uint16_t startDelayMs;

    // ── Color palette ─────────────────────────────────────────────────────────
    //    colorCount must be 1..BB8_MAX_COLORS.
    //    Colors advance on each OFF→ON transition (so the first blink = palette[0]).
    //    Single-color LEDs: set colorCount = 1 and only fill palette[0].
    uint8_t  colorCount;
    BB8Color palette[BB8_MAX_COLORS];

    // ── Runtime state (initialised by initBB8Matrix, do not set here) ─────────
    unsigned long nextToggleMs; // millis() timestamp of the next ON/OFF transition
    bool          isOn;         // current blink state
    uint8_t       colorIdx;     // which palette entry is currently active
};

// ── Global matrix (defined in bb8_sphere.cpp) ────────────────────────────────
extern BB8LedControl bb8_leds[BB8_LED_COUNT];

// ── Function declarations ─────────────────────────────────────────────────────
void initBB8Matrix();
bool updateBB8Matrix(Adafruit_NeoPixel &strip);
bool led_sphere_loop(Adafruit_NeoPixel &s);
