/*
 * effects.h
 *
 *  Created on: Jan 7, 2022
 *      Author: omwfo
 */

#ifndef INC_EFFECTS_H_
#define INC_EFFECTS_H_

#include "pixel.h"
#include "math.h"


#define DIM(c)     (uint32_t)((c >> 2) & 0x3f3f3f3f) // color at 25% intensity
#define DARK(c)    (uint32_t)((c >> 4) & 0x0f0f0f0f) // color at  6% intensity

void pixel_sine(ws2812_rgb_struct *_ws_struct);


#define HEARTBEAT_STEPS		16

void stripEffect_CircularRing(uint32_t interval, uint8_t red, uint8_t green,
		uint8_t blue);
void stripEffect_HeartBeat(uint32_t interval, uint8_t red, uint8_t green,
		uint8_t blue);
void stripEffect_ColorWheel(uint32_t interval);
void stripEffect_AllColors(uint32_t interval);
void stripEffect_PatternMove(uint32_t interval, uint32_t parts, uint8_t red,
		uint8_t green, uint8_t blue);
void stripEffect_FullEmpty(uint32_t interval, uint8_t red, uint8_t green,
		uint8_t blue);
void stripEffect_FromTo(uint32_t interval, uint32_t steps, uint8_t redA,
		uint8_t greenA, uint8_t blueA, uint8_t redB, uint8_t greenB,
		uint8_t blueB);
void stripEffect_AlternateColors(uint32_t interval, uint32_t steps,
		uint8_t redA, uint8_t greenA, uint8_t blueA, uint8_t redB,
		uint8_t greenB, uint8_t blueB);

void colour_scroll(ws2812_rgb_struct *_rgb_struct);
void hsv_scroll(ws2812_rgb_struct *_rgb_struct);
void shift_offset();



uint32_t color_wheel(uint16_t pos,uint16_t _intensity);


#endif /* INC_EFFECTS_H_ */
