#include <Arduino.h>
#include "led_sequences.h"

void setupLEDSequence() {
    // Function implementation
}

void stopLEDSequence() {
    // Function implementation
}

void solidFill(Adafruit_NeoPixel &s, uint32_t color, uint8_t start_led, uint8_t number_of_leds) {
  for(int i=start_led; i<= start_led + number_of_leds; i++) { // For each pixel in strip...
    s.setPixelColor(i, color);         //  Set pixel's color (in RAM)
  }
}
