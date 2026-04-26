#pragma once
#include <Arduino.h>

struct led_sparkleRandomAnim {
  uint16_t length = 0;       // number of LEDs in range
  uint16_t startIndex = 0;   // starting LED index
  uint8_t r, g, b, w;    // base color
  uint8_t count = 0;         // how many pixels to light (1 .. 100+) - will be randomized as the routine functions
  uint8_t minCount = 0;      // minimum number of random pixels to light
  uint8_t maxCount = 0;      // maximium number of random pixels to light
  uint16_t delay = 500;        // animation speed in ms - will be randomized as the routine functions
  uint16_t minDelay = 250;     // animation speed in ms - minimum for randomization
  uint16_t maxDelay = 750;     // animation speed in ms - maximum for randomization
  uint32_t lastUpdate;   // when was this last updated
};

void led_sparkleRandom(Adafruit_NeoPixel &s, led_sparkleRandomAnim &anim);
