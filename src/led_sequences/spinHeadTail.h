#pragma once
#include <Arduino.h>

struct led_SpinAnim {
  uint16_t ringLen = 0;     // size of the ring we're spinning around
  uint16_t startIndex = 0;  // pixel where this segment begins
  uint16_t trailLen = 0;    // how many pixels make the tail (1 = just head)
  uint8_t  r, g, b, w;  // base color
  uint16_t pos = 0;         // persistent step counter
  bool serialDebug = false;     // enable serial debugging
  uint16_t delay = 1000;        // animation speed in ms - will be randomized as the routine functions
  uint16_t minDelay = 0;     // animation speed in ms - minimum for randomization
  uint16_t maxDelay = 0;     // animation speed in ms - maximum for randomization
  uint32_t lastUpdate;   // when was this last updated

};

void led_spinHeadTail(Adafruit_NeoPixel &s, led_SpinAnim &anim);
