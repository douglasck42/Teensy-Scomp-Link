#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "led_sequences/led_sequences.h"

// ── Hardware ────────────────────────────────────────────────────────────────
#define PIN_STRIP1      32
#define PIN_STRIP2      33
#define PIN_ONBOARD      2

#define NUM_LEDS_STRIP1 64              // 64 LED Matrix 8x8
#define NUM_LEDS_STRIP2 8+24           // NeoPixel Stick Cool White + NeoPixel 24-Led Cool White

#define BRIGHTNESS      40   // 0–255, keep low on USB power

// ── Strip instances ─────────────────────────────────────────────────────────
// SK6812 RGBW — change to NEO_GRB + NEO_KHZ800 if your strips are RGB only
Adafruit_NeoPixel strip1(NUM_LEDS_STRIP1, PIN_STRIP1, NEO_GRB  + NEO_KHZ800);             // Random Head LEDs, future
Adafruit_NeoPixel strip2(NUM_LEDS_STRIP2, PIN_STRIP2, NEO_GRBW + NEO_KHZ800);             // NeoPixel Stick Cool White + NeoPixel Ring 24
Adafruit_NeoPixel onboard(1, PIN_ONBOARD, NEO_GRB + NEO_KHZ800);

#define HEARTBEAT_INTERVAL_MS 10000


// ── LED Stuff ─────────────────────────────────────────────────────────────────
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

// ── Helpers ─────────────────────────────────────────────────────────────────
void fillStrip(Adafruit_NeoPixel &strip, uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
    uint32_t color = strip.Color(r, g, b, w);
    for (int i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, color);
    }
    strip.show();
}

// ── Main ─────────────────────────────────────────────────────────────────────

void setup() {
    Serial.begin(115200);
    delay(250);
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

    show_all_sprites(square8by8, 1500, 500, 8);

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

}

void loop() {
    unsigned long now = millis();
    bool update_show = false;
    static unsigned long millis_lastHeartbeat = now;
    static uint32_t count_lastHeartbeat = 0;

    // timers, these get set to current millis() at various points in the code to manage timing of different functions and features
    static unsigned long millis_lastSbusRead = now;

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

    if (update_show) {
        strip1.show();
        strip2.show();
    }

    // Heartbeat
    if (now - millis_lastHeartbeat >= HEARTBEAT_INTERVAL_MS) {
        Serial.printf("Heartbeat: Teensy Scomp Link alive (%ds)\n", count_lastHeartbeat * HEARTBEAT_INTERVAL_MS / 1000);
        count_lastHeartbeat++;          // Unsigned int will eventually wrap, that's fine
        millis_lastHeartbeat = now;
    }

    delay(40);        // ~25fps

}