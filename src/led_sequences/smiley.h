#pragma once
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "led_sequences.h"

void led_smiley(Adafruit_NeoPixel &s, led_animation &anim);
void led_heart(Adafruit_NeoPixel &s, led_animation &anim);
