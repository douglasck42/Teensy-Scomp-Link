#include <Arduino.h>
#include "led_sequences.h"

// BB-8 CNC Sphere, 17 LEDs in one continous strip

// directions of clockwise are from the EXTERIOR of sphere
// Sphere order from LED Strip: 
// C6, C3, C5, C1, C2, C4 (Locking ring Panel)

// CIRCLE 6
#define C6_1 1    // Tighter group of three, clockwise first
#define C6_2 2    // Tighter group of three, clockwise middle
#define C6_3 3    // Tighter group of three, clockwise last
#define C6_4 4    // 2+1 group, clockwise first
#define C6_5 5    // 2+1 group, clockwise middle
#define C6_6 6    // 2+1 group, clockwise last
// CIRCLE 3
#define C3_7 7    // Pair, First LED, Narrow Rectangle
#define C3_8 8    // Pair, Second LED, Narrow Rectangle
// CIRCLE 5
#define C5_9 9    // Single, First LED, Narrow Rectangle
// CIRCLE 1
#define C1_10 10   // Clockwise, First LED, Narrow Rectangle
#define C1_11 11   // Clockwise, Second LED, Narrow Rectangle
#define C1_12 12   // Clockwise, Third LED, Narrow Rectangle
#define C1_13 13   // Clockwise, Fourth LED, Narrow Rectangle
// CIRCLE 2
#define C2_14 14   // Outer LED, Square
#define C2_15 15   // Center LED, Square
// CIRCLE 4 - Locking Ring Panel
#define C4_16 16   // Closest to center, Square
#define C4_17 17   // Furthest from center, Square

#define TOTAL_LEDS 17

#ifndef BB8_SPHERE_ONLY
#define BB8_SPHERE_ONLY 0
#endif

bool led_sphere_loop(Adafruit_NeoPixel &s) {
    #if BB8_SPHERE_ONLY == 0
    return; // disable this routine if not testing the sphere-only sequence
    #endif

    static unsigned long last_ms = 0;
    static uint8_t phase = 0;
    unsigned long now = millis();

    if (now - last_ms < 2000) return false;
    last_ms = now;

    LedStrip ls(s, false, false, 255, 0, TOTAL_LEDS);
    switch (phase) {
        case 0: solidFill(ls, HEX_red);
            Serial.println("LED Test: Red");
            break;  // red
        case 1: solidFill(ls, HEX_green);
            Serial.println("LED Test: Green");
            break;  // green
        case 2: solidFill(ls, HEX_blue);
            Serial.println("LED Test: Blue");
            break;  // blue
        case 3: solidFill(ls, HEX_white);
            Serial.println("LED Test: White (RGB mix)");
            break;  // white (RGB mix)
    }
    if (++phase > 3) phase = 0;
    return true;
}
