// Copyright (c) 2026 Douglas Kempthorne (douglas@kempthorne.com)
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "led_sequences/led_sequences.h"

// Forward declare your wrapper
struct LedStrip;

// Draw an 8x8 sprite at a given offset
void drawSprite(LedStrip &strip, const uint32_t sprite[8][8], int offset = 0);

// Optional: clear helper
void clearStrip(LedStrip &strip);

void show_all_sprites(LedStrip &s, uint16_t delay, uint16_t ani_delay, uint8_t ani_repeat);

// ===== SPRITE DECLARATIONS =====
extern const uint32_t SPRITE_EMPTY[8][8];
extern const uint32_t SPRITE_SMILEY[8][8];
extern const uint32_t SPRITE_HEART[8][8];
extern const uint32_t SPRITE_HEART_FANCY[8][8];
extern const uint32_t SPRITE_BONES[8][8];
extern const uint32_t SPRITE_FIREWORK_ORANGE_1[8][8]; 
extern const uint32_t SPRITE_FIREWORK_ORANGE_2[8][8]; 
extern const uint32_t SPRITE_STITCH[8][8]; 
extern const uint32_t SPRITE_BLUEBALL[8][8]; 
