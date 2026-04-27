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


void led_heart(Adafruit_NeoPixel &s, led_animation &anim) {
    if (anim.serialDebug) Serial.print("Wipe: ");

    // --- Step 1: turn OFF all LEDs in this range only
    for (uint16_t idx = anim.startIndex; idx < anim.length; idx++) {
        s.setPixelColor(idx, 0); // off
    }

    s.setPixelColor(2, s.Color(anim.r, anim.g, anim.b, anim.w));
    s.setPixelColor(3, s.Color(anim.r, anim.g, anim.b, anim.w));
    s.setPixelColor(5, s.Color(anim.r, anim.g, anim.b, anim.w));
    s.setPixelColor(6, s.Color(anim.r, anim.g, anim.b, anim.w));

    s.setPixelColor(9, s.Color(anim.r, anim.g, anim.b, anim.w));
    s.setPixelColor(10, s.Color(anim.r, anim.g, anim.b, anim.w));
    s.setPixelColor(11, s.Color(anim.r, anim.g, anim.b, anim.w));
    s.setPixelColor(12, s.Color(anim.r, anim.g, anim.b, anim.w));
    s.setPixelColor(13, s.Color(anim.r, anim.g, anim.b, anim.w));
    s.setPixelColor(14, s.Color(anim.r, anim.g, anim.b, anim.w));
    s.setPixelColor(15, s.Color(anim.r, anim.g, anim.b, anim.w));

    s.setPixelColor(17, s.Color(anim.r, anim.g, anim.b, anim.w));
    s.setPixelColor(18, s.Color(anim.r, anim.g, anim.b, anim.w));
    s.setPixelColor(19, s.Color(anim.r, anim.g, anim.b, anim.w));
    s.setPixelColor(20, s.Color(anim.r, anim.g, anim.b, anim.w));
    s.setPixelColor(21, s.Color(anim.r, anim.g, anim.b, anim.w));
    s.setPixelColor(22, s.Color(anim.r, anim.g, anim.b, anim.w));
    s.setPixelColor(23, s.Color(anim.r, anim.g, anim.b, anim.w));

    s.setPixelColor(25, s.Color(anim.r, anim.g, anim.b, anim.w));
    s.setPixelColor(26, s.Color(anim.r, anim.g, anim.b, anim.w));
    s.setPixelColor(27, s.Color(anim.r, anim.g, anim.b, anim.w));
    s.setPixelColor(28, s.Color(anim.r, anim.g, anim.b, anim.w));
    s.setPixelColor(29, s.Color(anim.r, anim.g, anim.b, anim.w));
    s.setPixelColor(30, s.Color(anim.r, anim.g, anim.b, anim.w));
    s.setPixelColor(31, s.Color(anim.r, anim.g, anim.b, anim.w));

    s.setPixelColor(33, s.Color(anim.r, anim.g, anim.b, anim.w));
    s.setPixelColor(34, s.Color(anim.r, anim.g, anim.b, anim.w));
    s.setPixelColor(35, s.Color(anim.r, anim.g, anim.b, anim.w));
    s.setPixelColor(36, s.Color(anim.r, anim.g, anim.b, anim.w));
    s.setPixelColor(37, s.Color(anim.r, anim.g, anim.b, anim.w));
    s.setPixelColor(38, s.Color(anim.r, anim.g, anim.b, anim.w));
    s.setPixelColor(39, s.Color(anim.r, anim.g, anim.b, anim.w));

    s.setPixelColor(42, s.Color(anim.r, anim.g, anim.b, anim.w));
    s.setPixelColor(43, s.Color(anim.r, anim.g, anim.b, anim.w));
    s.setPixelColor(44, s.Color(anim.r, anim.g, anim.b, anim.w));
    s.setPixelColor(45, s.Color(anim.r, anim.g, anim.b, anim.w));
    s.setPixelColor(46, s.Color(anim.r, anim.g, anim.b, anim.w));

    s.setPixelColor(51, s.Color(anim.r, anim.g, anim.b, anim.w));
    s.setPixelColor(52, s.Color(anim.r, anim.g, anim.b, anim.w));
    s.setPixelColor(53, s.Color(anim.r, anim.g, anim.b, anim.w));

    s.setPixelColor(60, s.Color(anim.r, anim.g, anim.b, anim.w));

}
