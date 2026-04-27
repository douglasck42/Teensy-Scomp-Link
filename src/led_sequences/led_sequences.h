#pragma once
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

void setupLEDSequence();
void stopLEDSequence();
void solidFill(Adafruit_NeoPixel &s, uint32_t color, uint8_t start_led, uint8_t number_of_leds);

struct led_animation {
  uint16_t startIndex = 0;   // starting LED Number, 0-Indexed
  uint16_t length = 0;       // number of LEDs in range
  uint16_t delay = 500;      // animation speed in ms - will be randomized as the routine functions
  uint16_t minDelay = 500;   // animation speed in ms - minimum for randomization - as shown, a 0ms variation
  uint16_t maxDelay = 500;   // animation speed in ms - maximum for randomization - as shown, a 0ms variation
  unsigned long lastUpdate = 0;   // when was this last updated
  bool serialDebug = false;     // enable serial debugging
  bool enabled = false;      // run this animation

  // base color
  uint8_t r = 0;
  uint8_t g = 0;
  uint8_t b = 0;
  uint8_t w = 0;    
};

#include "sparkle_random.h"
#include "spinHeadTail.h"
#include "breathe.h"
#include "game_of_life.h"
#include "smiley.h"
