/*
 * pixel.h
 *
 *  Created on: Nov 4, 2021
 *      Author: omwfo
 */

#ifndef INC_PIXEL_H_
#define INC_PIXEL_H_
#include <stdint.h>
#include <stdbool.h>

#define NUMBER_OF_PIXELS    		2
#define WORDS_PER_PIXEL				24
#define NUMBER_OF_PIXELS_HALF  		(NUMBER_OF_PIXELS/2)
#define ZERO_PADDING 				42
#define BUFFER_LENGTH 				(((NUMBER_OF_PIXELS * WORDS_PER_PIXEL)  + ZERO_PADDING) * 2)


typedef struct xRGB {
	uint8_t unused;
	uint8_t red;
	uint8_t green;
	uint8_t blue;
}XRGB;


typedef union uint32_RGB
{
	XRGB	  xRGB;
	uint32_t  xUINT;
} UINT32_RGB;



void drawFrame(void);
void clearAll(void);
void getPixelColor(int px_index, UINT32_RGB * px_color);
void setPixel_GRB(const UINT32_RGB * Color, int px_index);
void setAll_GRB(const UINT32_RGB * color);
void setRange_GRB(const UINT32_RGB * color, int start_px_index, int len);
void setColorBrightness(const UINT32_RGB * in, UINT32_RGB * out, float brightness);


void RunningLights(const UINT32_RGB * c, int delay_ms, float time_s);
void FadeInOut(const UINT32_RGB * c, int delay_ms, float time_s);
void CyloneBounce(const UINT32_RGB *c, int delay_ms, int eye_size);
void Twinkle(const UINT32_RGB *c, int delay_ms, int count, bool clear);
void ColorWipe(const UINT32_RGB * c, int delay_ms, bool reverse);
void RainbowCycle(int delay_ms, int cycle_num);
void TheatreChase(const UINT32_RGB * c, int delay_ms, int cycle_num);
void TheatreChaseRainbow(int delay_ms, int cycle_num);
void Fire(int cooling_rate, int sparking_rate, int delay_ms);
void meteorRain(const UINT32_RGB * c, uint8_t meteorSize, uint8_t meteorTrailDecay, bool meteorRandomDecay, int delay_ms);
void HalfBlink(const UINT32_RGB * c, int delay_ms);


#endif /* INC_PIXEL_H_ */
