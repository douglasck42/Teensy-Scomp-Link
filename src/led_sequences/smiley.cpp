#include "smiley.h"
#include "led_sequences.h"

void led_smiley(Adafruit_NeoPixel &s, led_animation &anim) {
    if (anim.serialDebug) Serial.print("Wipe: ");

    // --- Step 1: turn OFF all LEDs in this range only
    for (uint16_t idx = anim.startIndex; idx < anim.length; idx++) {
        s.setPixelColor(idx, 0); // off
    }

    s.setPixelColor(2, s.Color(anim.r, anim.g, anim.b, anim.w));
    s.setPixelColor(5, s.Color(anim.r, anim.g, anim.b, anim.w));

    s.setPixelColor(10, s.Color(anim.r, anim.g, anim.b, anim.w));
    s.setPixelColor(13, s.Color(anim.r, anim.g, anim.b, anim.w));

    s.setPixelColor(33, s.Color(anim.r, anim.g, anim.b, anim.w));
    s.setPixelColor(38, s.Color(anim.r, anim.g, anim.b, anim.w));

    s.setPixelColor(41, s.Color(anim.r, anim.g, anim.b, anim.w));
    s.setPixelColor(46, s.Color(anim.r, anim.g, anim.b, anim.w));

    s.setPixelColor(50, s.Color(anim.r, anim.g, anim.b, anim.w));
    s.setPixelColor(51, s.Color(anim.r, anim.g, anim.b, anim.w));
    s.setPixelColor(52, s.Color(anim.r, anim.g, anim.b, anim.w));
    s.setPixelColor(53, s.Color(anim.r, anim.g, anim.b, anim.w));

    s.setPixelColor(59, s.Color(anim.r, anim.g, anim.b, anim.w));
    s.setPixelColor(60, s.Color(anim.r, anim.g, anim.b, anim.w));

}
