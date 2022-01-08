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




uint32_t color_wheel(uint16_t pos);


#endif /* INC_EFFECTS_H_ */
