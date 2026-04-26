#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

// ── Hardware ────────────────────────────────────────────────────────────────
#define PIN_STRIP1      33
#define PIN_STRIP2      32
#define PIN_ONBOARD      2

#define NUM_LEDS_STRIP1 24
#define NUM_LEDS_STRIP2 8           // NeoPixel Stick Cool White

#define BRIGHTNESS      40   // 0–255, keep low on USB power

// ── Strip instances ─────────────────────────────────────────────────────────
// SK6812 RGBW — change to NEO_GRB + NEO_KHZ800 if your strips are RGB only
Adafruit_NeoPixel strip1(NUM_LEDS_STRIP1, PIN_STRIP1, NEO_GRB  + NEO_KHZ800);              // NeoPixel Ring 24
Adafruit_NeoPixel strip2(NUM_LEDS_STRIP2, PIN_STRIP2, NEO_GRBW + NEO_KHZ800);             // NeoPixel Stick Cool White
Adafruit_NeoPixel onboard(1, PIN_ONBOARD, NEO_GRB + NEO_KHZ800);

// ── Helpers ─────────────────────────────────────────────────────────────────

// HSV → RGBW: hue 0–359, sat/val 0–255. W channel used only at low saturation.
uint32_t hsvToRGBW(uint16_t hue, uint8_t sat, uint8_t val) {
    // Get RGB from the library's HSV helper (returns packed RGB, not RGBW)
    uint32_t rgb = Adafruit_NeoPixel::ColorHSV(hue * 182, sat, val);
    uint8_t r = (rgb >> 16) & 0xFF;
    uint8_t g = (rgb >>  8) & 0xFF;
    uint8_t b = (rgb      ) & 0xFF;
    // Derive white from desaturation headroom
    uint8_t w = (uint8_t)(255 - sat) * val / 255;
    return Adafruit_NeoPixel::Color(r, g, b, w);
}

// ── Strip 1 — bouncing warm-white ping-pong (8 LEDs) ────────────────────────
// A warm white dot bounces back and forth, leaving a short decaying tail.
// Uses the W channel directly to show RGBW capability.

namespace s1 {
    float pos    = 0.0f;
    float vel    = 0.12f;   // pixels per frame
    uint8_t tail[NUM_LEDS_STRIP1] = {};
}

void updateStrip1() {
    s1::pos += s1::vel;
    if (s1::pos >= NUM_LEDS_STRIP1 - 1 || s1::pos <= 0) {
        s1::vel = -s1::vel;
        s1::pos = constrain(s1::pos, 0.0f, (float)(NUM_LEDS_STRIP1 - 1));
    }

    // Decay tail
    for (int i = 0; i < NUM_LEDS_STRIP1; i++) {
        s1::tail[i] = s1::tail[i] > 20 ? s1::tail[i] - 20 : 0;
    }

    // Stamp head — pure warm white (W channel only, slight amber R tint)
    int head = (int)s1::pos;
    s1::tail[head] = 255;

    for (int i = 0; i < NUM_LEDS_STRIP1; i++) {
        uint8_t w = s1::tail[i];
        uint8_t r = w / 6;   // slight amber warmth
        strip1.setPixelColor(i, Adafruit_NeoPixel::Color(r, 0, 0, w));
    }
    strip1.show();
}

// ── Strip 2 — hue wave (24 LEDs) ────────────────────────────────────────────
// A slow rainbow wave scrolls across the strip. W channel tracks desaturation
// so you can see it engage as colors wash out toward white.

namespace s2 {
    uint16_t offset = 0;
}

void updateStrip2() {
    for (int i = 0; i < NUM_LEDS_STRIP2; i++) {
        uint16_t hue = (s2::offset + i * (360 / NUM_LEDS_STRIP2)) % 360;
        strip2.setPixelColor(i, hsvToRGBW(hue, 200, 255));
    }
    strip2.show();
    s2::offset = (s2::offset + 1) % 360;
}

// ── Onboard — slow hue cycle ─────────────────────────────────────────────────

void updateOnboard() {
    static uint16_t hue = 0;
    onboard.setPixelColor(0, onboard.ColorHSV(hue));
    onboard.show();
    hue += 200;
}

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
    // test red
    r=255; g=0; b=0; w=0;
    fillStrip(strip1, r, g, b, w);
    fillStrip(strip2, r, g, b, w);
    delay(2000);
    // test green
    r=0; g=255; b=0; w=0;
    fillStrip(strip1, r, g, b, w);
    fillStrip(strip2, r, g, b, w);
    delay(2000);
    // test blue
    r=0; g=0; b=255; w=0;
    fillStrip(strip1, r, g, b, w);
    fillStrip(strip2, r, g, b, w);
    delay(2000);
    // test white
    r=0; g=0; b=0; w=255;
    fillStrip(strip1, r, g, b, w);
    fillStrip(strip2, r, g, b, w);
    delay(2000);

}

void loop() {
    updateStrip1();   // bouncing warm-white dot
    updateStrip2();   // scrolling rainbow wave
    updateOnboard();  // hue cycle

    delay(20);        // ~50fps
}