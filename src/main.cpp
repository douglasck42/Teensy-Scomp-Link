#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "led_sequences/sequences.h"

// ── Hardware ────────────────────────────────────────────────────────────────
#define PIN_STRIP1      33
#define PIN_STRIP2      32
#define PIN_ONBOARD      2

#define NUM_LEDS_STRIP1 1
#define NUM_LEDS_STRIP2 8+24           // NeoPixel Stick Cool White + NeoPixel 24-Led Cool White

#define BRIGHTNESS      40   // 0–255, keep low on USB power

// ── Strip instances ─────────────────────────────────────────────────────────
// SK6812 RGBW — change to NEO_GRB + NEO_KHZ800 if your strips are RGB only
Adafruit_NeoPixel strip1(NUM_LEDS_STRIP1, PIN_STRIP1, NEO_GRB  + NEO_KHZ800);             // Random Head LEDs, future
Adafruit_NeoPixel strip2(NUM_LEDS_STRIP2, PIN_STRIP2, NEO_GRBW + NEO_KHZ800);             // NeoPixel Stick Cool White + NeoPixel Ring 24
Adafruit_NeoPixel onboard(1, PIN_ONBOARD, NEO_GRB + NEO_KHZ800);

// ── Helpers ─────────────────────────────────────────────────────────────────
led_sparkleRandomAnim neo_pixel_strip = {};


led_SpinAnim lg_eye_out_spin = {};

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
    Serial.println("Sparkle Motion Mini — Red test");
    // test red
    r=255; g=0; b=0; w=0;
    //fillStrip(strip1, r, g, b, w);
    fillStrip(strip2, r, g, b, w);
    delay(1000);
    Serial.println("Sparkle Motion Mini — Green test");
    // test green
    r=0; g=255; b=0; w=0;
    //fillStrip(strip1, r, g, b, w);
    fillStrip(strip2, r, g, b, w);
    delay(1000);
    Serial.println("Sparkle Motion Mini — Blue test");
    // test blue
    r=0; g=0; b=255; w=0;
    //fillStrip(strip1, r, g, b, w);
    fillStrip(strip2, r, g, b, w);
    delay(1000);
    Serial.println("Sparkle Motion Mini — RGB test");
    // test white
    r=255; g=255; b=255; w=0;
    fillStrip(strip1, r, g, b, w);
    fillStrip(strip2, r, g, b, w);
    delay(1000);
    Serial.println("Sparkle Motion Mini — White test");
    // test white
    r=0; g=0; b=0; w=255;
    //fillStrip(strip1, r, g, b, w);
    fillStrip(strip2, r, g, b, w);
    delay(1000);
    Serial.println("Sparkle Motion Mini — All Off");
    // test white
    r=0; g=0; b=0; w=0;
    fillStrip(strip1, r, g, b, w);
    fillStrip(strip2, r, g, b, w);
    delay(50);

    // Configure the LED Strip Eye Animation
    neo_pixel_strip.length = 6;       // number of LEDs in range
    neo_pixel_strip.startIndex = 1;   // starting LED index
    neo_pixel_strip.r = 0;
    neo_pixel_strip.g = 0;
    neo_pixel_strip.b = 255;
    neo_pixel_strip.w = 0;
    neo_pixel_strip.count = 3;      // how many pixels to light (1 .. 100+) - will be randomized as the routine functions
    neo_pixel_strip.minCount = 1;      // minimum number of random pixels to light
    neo_pixel_strip.maxCount = 6;      // maximium number of random pixels to light
    neo_pixel_strip.delay = 875;        // animation speed in ms - will be randomized as the routine functions
    neo_pixel_strip.minDelay = 750;     // animation speed in ms - minimum for randomization
    neo_pixel_strip.maxDelay = 3000;     // animation speed in ms - maximum for randomization

    // Configure the Outer Ring Eye Animation
    lg_eye_out_spin.ringLen = 24;
    lg_eye_out_spin.startIndex = 8;
    lg_eye_out_spin.trailLen = 12;
    lg_eye_out_spin.r = 32;
    lg_eye_out_spin.g = 32;
    lg_eye_out_spin.b = 129;
    lg_eye_out_spin.w = 16;
    lg_eye_out_spin.delay = 500;        // animation speed in ms - will be randomized as the routine functions

}

void loop() {
    unsigned long now = millis();

    // timers, these get set to current millis() at various points in the code to manage timing of different functions and features
    static unsigned long millis_lastSbusRead = now;

    if (now - neo_pixel_strip.lastUpdate >= neo_pixel_strip.delay) {
        neo_pixel_strip.lastUpdate = now;
        led_sparkleRandom(strip2, neo_pixel_strip);
    }
    if (now - lg_eye_out_spin.lastUpdate >= lg_eye_out_spin.delay) {
        lg_eye_out_spin.lastUpdate = now;
        led_spinHeadTail(strip2, lg_eye_out_spin); // drive our spinner
    }

    strip2.show();


    delay(40);        // ~25fps
}