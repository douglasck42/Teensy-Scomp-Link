// Copyright (c) 2026 Douglas Kempthorne (douglas@kempthorne.com)
// SPDX-License-Identifier: GPL-3.0-or-later
#define BUILD_VERSION "0.3.0"

#define BUILD_VERSION "0.3.0"

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "common/settings.h"
#include "common/common.h"
#include "led_sequences/led_sequences.h"
#include "scomp/scomp_link.h"
#include <HardwareSerial.h>

// ========================= HARDWARE PINS =========================
#define PIN_STRIP1      32
#define PIN_STRIP2      33
#define PIN_ONBOARD      2

#ifndef PIN_SERIAL_RX
#define PIN_SERIAL_RX  26
#endif
#ifndef PIN_SERIAL_TX
#define PIN_SERIAL_TX  25
#endif

// ========================= SERIAL SCOMP INIT =========================
HardwareSerial scompSerialPort(1);   // UART1 (UART0 is the USB debug port)
ScompLink scomp;

struct StripTypeDef { const char* name; neoPixelType type; };
static const StripTypeDef STRIP_TYPES[] = {
    { "GRB_800",  NEO_GRB  + NEO_KHZ800 },   // SCOMP_STRIP_GRB_800  — WS2812B
    { "RGB_800",  NEO_RGB  + NEO_KHZ800 },   // SCOMP_STRIP_RGB_800
    { "GRBW_800", NEO_GRBW + NEO_KHZ800 },   // SCOMP_STRIP_GRBW_800 — SK6812
    { "RGBW_800", NEO_RGBW + NEO_KHZ800 },   // SCOMP_STRIP_RGBW_800
    { "GRB_400",  NEO_GRB  + NEO_KHZ400 },   // SCOMP_STRIP_GRB_400
    { "RGB_400",  NEO_RGB  + NEO_KHZ400 },   // SCOMP_STRIP_RGB_400
};
static const uint8_t STRIP_TYPES_COUNT = sizeof(STRIP_TYPES) / sizeof(STRIP_TYPES[0]);

// ========================= LED SETTINGS =========================
#define NUM_LEDS_STRIP1 64              // 64 LED Matrix 8x8
#define NUM_LEDS_STRIP2 8+24           // NeoPixel Stick Cool White + NeoPixel 24-Led Cool White
#define BRIGHTNESS      40   // 0–255, keep low on USB power
// Strip instances
// SK6812 RGBW — change to NEO_GRB + NEO_KHZ800 if your strips are RGB only
Adafruit_NeoPixel strip1(NUM_LEDS_STRIP1, PIN_STRIP1, NEO_GRB  + NEO_KHZ800);             // Random Head LEDs, future
Adafruit_NeoPixel strip2(NUM_LEDS_STRIP2, PIN_STRIP2, NEO_GRBW + NEO_KHZ800);             // NeoPixel Stick Cool White + NeoPixel Ring 24
Adafruit_NeoPixel onboard(1, PIN_ONBOARD, NEO_GRB + NEO_KHZ800);

static void onScompStripConfig(const ScompStripConfig &msg) {
    if (msg.strip1_type_index < STRIP_TYPES_COUNT) {
        strip1.updateLength(msg.strip1_num_leds);
        strip1.setPin(msg.strip1_pin);
        strip1.updateType(STRIP_TYPES[msg.strip1_type_index].type);
        strip1.setBrightness(msg.strip1_brightness);
        strip1.clear();
        strip1.show();
    }
    if (msg.strip2_type_index < STRIP_TYPES_COUNT) {
        strip2.updateLength(msg.strip2_num_leds);
        strip2.setPin(msg.strip2_pin);
        strip2.updateType(STRIP_TYPES[msg.strip2_type_index].type);
        strip2.setBrightness(msg.strip2_brightness);
        strip2.clear();
        strip2.show();
    }
}

static void onScompInChannels(const ScompInputChannels &msg) {
    for (uint8_t i = 0; i < SCOMP_IN_CH; i++) {
        settings.ichannel[i].us_value = msg.us[i];
        settings.ichannel[i].updated  = true;
    }
}

static void onScompOutChannels(const ScompOutputChannels &msg) {
    for (uint8_t i = 0; i < SCOMP_OUT_CH; i++) {
        settings.ochannel[i].us_value = msg.us[i];
        settings.ochannel[i].updated  = true;
    }
}

// ========================= HEARTBEAT SETTINGS =========================
#define HEARTBEAT_INTERVAL_MS 5000
static unsigned long millis_lastHeartbeat = 0;

#define NEOPIXEL_MAX_FPS 25
#define NEOPIXEL_MAX_DELAY_MS (1000 / NEOPIXEL_MAX_FPS)

LedStrip square8by8(strip1, false, false, 128, 0, 64);
LedStrip radarEyeStrip(strip2, false, true, 128, 1, 6);
LedStrip radarEyeRing(strip2, false, true, 128, 8, 64);

//LedStrip square8by8 = {
//    strip1,     // NoeoPixel strip identifier
//    false,      // isRGBW
//    true,       // Use Gamma Correction
//    128,        // Brightness
//    0,          // Starting Index
//    64          // Length
//};
//LedStrip radarEyeStrip = {
//    strip2,     // NoeoPixel strip identifier
//    true,       // isRGBW
//    false,      // Use Gamma Correction
//    128,        // Brightness
//    1,          // Starting Index - technically 0 but it's obscured
//    6           // Length - technically 8, both ends obscured
//};
//LedStrip radarEyeRing = {
//    strip2,     // NoeoPixel strip identifier
//    true,       // isRGBW
//    false,      // Use Gamma Correction
//    128,        // Brightness
//    8,          // Starting Index
//    24          // Length
//};

led_sparkleRandomAnim square8by8_sparkle = {};
led_sparkleRandomAnim radarEyeStrip_sparkle = {};
led_SpinAnim radarEyeRing_spin = {};
led_breatheAnim radarEyeRing_breathe = {};
led_gameOfLife square8by8_gameOfLife = {};
led_animation smiley = {};

// ========================= fillStrip() =========================
void fillStrip(Adafruit_NeoPixel &strip, uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
    uint32_t color = strip.Color(r, g, b, w);
    for (int i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, color);
    }
    strip.show();
}

// ── Setup  ─────────────────────────────────────────────────────────────────────
void setup() {
    Serial.begin(115200);
#if WAIT_FOR_SERIAL == 1
    if (WAIT_FOR_SERIAL) {
        while (!Serial) {
            delay(100);
        }
    }

    Serial.println("Scomp Link v" BUILD_VERSION ": starting up... (serial required)");
#else
    delay(2000); // Wait for Serial to be ready but non-blocking
    Serial.println("Scomp Link v" BUILD_VERSION ": starting up... (serial optional)");
#endif

    scompSerialPort.setRxBufferSize(2048);
    scompSerialPort.begin(SCOMP_BAUD_RATE, SERIAL_8N1, PIN_SERIAL_RX, PIN_SERIAL_TX);
    scomp.begin(scompSerialPort, SCOMP_FLAG_NODE_LOCAL);
    scomp.onInputChannels(onScompInChannels);
    scomp.onOutputChannels(onScompOutChannels);
    //scomp.onStripConfig(onScompStripConfig);

    Serial.println("Sparkle Motion Mini — RGBW test");

    strip1.begin();
    strip1.setBrightness(BRIGHTNESS);
    strip1.clear();
    strip1.show();

    strip2.begin();
    strip2.setBrightness(BRIGHTNESS);
    strip2.clear();
    strip2.show();

    onboard.begin();
    onboard.setBrightness(BRIGHTNESS);
    onboard.clear();
    onboard.show();


    u_int8_t r = 0;
    u_int8_t g = 0;
    u_int8_t b = 0;
    u_int8_t w = 0;

    #define RGB_TEST_DELAY 750
    #define RGB_TEST_ENABLED 0
    #if RGB_TEST_ENABLED == 1
    Serial.println("Sparkle Motion Mini — Red test");
    // test red
    r=128; g=0; b=0; w=0;
    //fillStrip(strip1, r, g, b, w);
    fillStrip(strip1, r, g, b, w);
    fillStrip(strip2, r, g, b, w);
    delay(RGB_TEST_DELAY);
    Serial.println("Sparkle Motion Mini — Green test");
    // test green
    r=0; g=128; b=0; w=0;
    fillStrip(strip1, r, g, b, w);
    fillStrip(strip2, r, g, b, w);
    delay(RGB_TEST_DELAY);
    Serial.println("Sparkle Motion Mini — Blue test");
    // test blue
    r=0; g=0; b=128; w=0;
    fillStrip(strip1, r, g, b, w);
    fillStrip(strip2, r, g, b, w);
    delay(RGB_TEST_DELAY);
    Serial.println("Sparkle Motion Mini — RGB White test");
    // test white
    r=128; g=128; b=128; w=0;
    fillStrip(strip1, r, g, b, w);
    fillStrip(strip2, r, g, b, w);
    delay(RGB_TEST_DELAY);
    Serial.println("Sparkle Motion Mini — White test");
    // test white
    r=0; g=0; b=0; w=128;
    fillStrip(strip1, r, g, b, w);
    fillStrip(strip2, r, g, b, w);
    delay(RGB_TEST_DELAY);
    Serial.println("Sparkle Motion Mini — All Off");
    // test white
    r=0; g=0; b=0; w=0;
    fillStrip(strip1, r, g, b, w);
    fillStrip(strip2, r, g, b, w);
    delay(25);
    #endif

    //show_all_sprites(square8by8, 1500, 500, 8);

    // square8by8 - Sparkle
    square8by8_sparkle.length = 64;       // number of LEDs in range
    square8by8_sparkle.startIndex = 0;   // starting LED index
    square8by8_sparkle.r = 0;
    square8by8_sparkle.g = 64;
    square8by8_sparkle.b = 64;
    square8by8_sparkle.w = 0;
    square8by8_sparkle.count = 20;      // how many pixels to light (1 .. 100+) - will be randomized as the routine functions
    square8by8_sparkle.minCount = 8;      // minimum number of random pixels to light
    square8by8_sparkle.maxCount = 20;      // maximium number of random pixels to light
    square8by8_sparkle.delay = 875;        // animation speed in ms - will be randomized as the routine functions
    square8by8_sparkle.minDelay = 750;     // animation speed in ms - minimum for randomization
    square8by8_sparkle.maxDelay = 3000;     // animation speed in ms - maximum for randomization
    square8by8_sparkle.enabled = false;

    // square8by8 - Game of Life
    square8by8_gameOfLife.width = 8;
    square8by8_gameOfLife.height = 8;
    square8by8_gameOfLife.startIndex = 0;
    square8by8_gameOfLife.length = 64;
    square8by8_gameOfLife.g = 32;
    square8by8_gameOfLife.b = 32;
    square8by8_gameOfLife.enabled = true;
    square8by8_gameOfLife.serialDebug = false;
    square8by8_gameOfLife.delay = 1250;

    // RadarEyeStrip - Sparkle
    radarEyeStrip_sparkle.length = 6;       // number of LEDs in range
    radarEyeStrip_sparkle.startIndex = 1;   // starting LED index
    radarEyeStrip_sparkle.r = 0;
    radarEyeStrip_sparkle.g = 0;
    radarEyeStrip_sparkle.b = 255;
    radarEyeStrip_sparkle.w = 0;
    radarEyeStrip_sparkle.count = 3;      // how many pixels to light (1 .. 100+) - will be randomized as the routine functions
    radarEyeStrip_sparkle.minCount = 1;      // minimum number of random pixels to light
    radarEyeStrip_sparkle.maxCount = 3;      // maximium number of random pixels to light
    radarEyeStrip_sparkle.delay = 875;        // animation speed in ms - will be randomized as the routine functions
    radarEyeStrip_sparkle.minDelay = 750;     // animation speed in ms - minimum for randomization
    radarEyeStrip_sparkle.maxDelay = 3000;     // animation speed in ms - maximum for randomization
    radarEyeStrip_sparkle.enabled = true;

    // RadarEyeRing - Spin
    radarEyeRing_spin.length = 24;
    radarEyeRing_spin.startIndex = 8;
    radarEyeRing_spin.trailLen = 12;
    radarEyeRing_spin.r = 32;
    radarEyeRing_spin.g = 32;
    radarEyeRing_spin.b = 129;
    radarEyeRing_spin.w = 16;
    radarEyeRing_spin.delay = 500;        // animation speed in ms - will be randomized as the routine functions
    radarEyeRing_spin.enabled = true;

    // RadarEyeRing - Breathe
    radarEyeRing_breathe.length = 24;
    radarEyeRing_breathe.startIndex = 8;
    radarEyeRing_breathe.r = 0;
    radarEyeRing_breathe.g = 0;
    radarEyeRing_breathe.b = 255;
    radarEyeRing_breathe.w = 0;
    radarEyeRing_breathe.delay = 50;        // animation speed in ms - needs to match defaultDelay
    radarEyeRing_breathe.defaultDelay = 50;        // animation speed in ms - needs to match defaultDelay
    radarEyeRing_breathe.enabled = false;

    print_memory_info();

}



void loop() {
    // timers, these get set to current millis() at various points in the code to manage timing of different functions and features
    unsigned long now = millis();
    static unsigned long millis_lastSbusRead = now;
    static unsigned long millis_lastLedShow = now;
    static unsigned long millis_lastScompSend = now;
    static unsigned long millis_lastScompHeartbeat = now;
    static unsigned long millis_lastPrintAll    = now;
    static uint32_t count_lastHeartbeat = 0;

    // do we update the LEDs this loop?
    // this does NOT change the rate limiter based on millis()
    bool update_show = false;

    // watchdog reset to prevent system hangs, especially important if the SD card is missing or there's an issue with the DFPlayer that could cause blocking calls
    #if WATCHDOG_ENABLED
    esp_task_wdt_reset();
    #endif

    // Scomp update to read incoming messages and trigger callbacks - this should be called every loop tick to ensure timely processing of incoming Scomp messages from the ESP32
    scomp.update();

    #if BB8_SPHERE_ONLY == 1
    update_show = led_sphere_loop(strip1);
    #else
    // Heartbeat (Scomp) — announce ourselves to the peer
    if (now - millis_lastScompHeartbeat >= HEARTBEAT_INTERVAL_MS) {
        scomp.sendHeartbeat(now);
        millis_lastScompHeartbeat = now;
    }

    // Periodic channel state push to SCOMP REMOTE — interleaved to halve per-tick burst size
    if (now - millis_lastScompSend >= SCOMP_SEND_INTERVAL_MS) {
        millis_lastScompSend = now;
        static bool send_input_next = true;
        if (send_input_next) {
            ScompInputChannels in_msg = {};
            for (uint8_t i = 0; i < SCOMP_IN_CH; i++) in_msg.us[i] = settings.ichannel[i].us_value;
            scomp.sendInputChannels(in_msg);
        } else {
            ScompOutputChannels out_msg = {};
            for (uint8_t i = 0; i < SCOMP_OUT_CH; i++) out_msg.us[i] = settings.ochannel[i].us_value;
            scomp.sendOutputChannels(out_msg);
        }
        send_input_next = !send_input_next;
    }

    // Heartbeat (USB Serial) — periodic status print
    if (now - millis_lastHeartbeat >= HEARTBEAT_INTERVAL_MS) {
        millis_lastHeartbeat = now;
        scomp.printHeartbeat();
    }

    #define PRINT_ALL_INTERVAL_MS 10000
    // verbose chatty stuff
    if (now - millis_lastPrintAll >= PRINT_ALL_INTERVAL_MS) {
        printChannelUs(ChannelType::iCHANNEL);
        printChannelUs(ChannelType::oCHANNEL);
        millis_lastPrintAll = now;
    }

    // square8by8 - Sparkle
    if (now - square8by8_sparkle.lastUpdate >= square8by8_sparkle.delay) {
        square8by8_sparkle.lastUpdate = now;
        if (square8by8_sparkle.enabled) {
            led_sparkleRandom(strip1, square8by8_sparkle);
            update_show = true;
        }
    }

    // square8by8 - Game of Life
    if (now - square8by8_gameOfLife.lastUpdate >= square8by8_gameOfLife.delay) {
        square8by8_gameOfLife.lastUpdate = now;
        if (square8by8_gameOfLife.enabled) {
            gameOfLifeRenderGrid(strip1, square8by8_gameOfLife);
            gameOfLifeStep(square8by8_gameOfLife);
            if (gameOfLifeIsDead(square8by8_gameOfLife)) {
                square8by8_gameOfLife.lastUpdateGameOfLifeReset = now;
                gameOfLifeInitGrid(square8by8_gameOfLife);
            }
            update_show = true;
        }
    }

    // RadarEyeStrip - Sparkle
    if (now - radarEyeStrip_sparkle.lastUpdate >= radarEyeStrip_sparkle.delay) {
        radarEyeStrip_sparkle.lastUpdate = now;
        if (radarEyeStrip_sparkle.enabled) {
            led_sparkleRandom(strip2, radarEyeStrip_sparkle);
            update_show = true;
        }
    }
    // RadarEyeRing - Spin
    if (now - radarEyeRing_spin.lastUpdate >= radarEyeRing_spin.delay) {
        radarEyeRing_spin.lastUpdate = now;
        if (radarEyeRing_spin.enabled) {
            led_spinHeadTail(strip2, radarEyeRing_spin); // drive our spinner
            update_show = true;
        }
    }
    // RadarEyeRing - Breathe
    if (now - radarEyeRing_breathe.lastUpdate >= radarEyeRing_breathe.delay) {
        radarEyeRing_breathe.lastUpdate = now;
        if (radarEyeRing_breathe.enabled) {
            led_breathe(strip2, radarEyeRing_breathe); // drive our spinner
            update_show = true;
        }
    }
    #endif

    if (update_show) {
        if (now - millis_lastLedShow >= NEOPIXEL_MAX_DELAY_MS) {   // limit show updates to ~25fps to avoid overwhelming the NeoPixel timing
            millis_lastLedShow = now;
            strip1.show();
            strip2.show();
        }
    }

}