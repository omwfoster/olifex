/*
 * effects.c
 *
 *
 *
 *
 *  Created on: Jan 7, 2022
 *      Author: omwfo
 */
#include "effects.h"

void pixel_sine(ws2812_rgb_struct *_ws_struct) {

}

/*
 * Put a value 0 to 255 in to get a color value.
 * The colours are a transition r -> g -> b -> back to r
 * Inspired by the Adafruit examples.
 */

uint32_t color_wheel(uint16_t pos, uint16_t _intensity) {
	pos = 255 - pos;
	if (pos < 85) {
		return ((uint32_t) (_intensity - pos * 3) << 16) | ((uint32_t) (0) << 8)
				| (pos * 3);
	} else if (pos < 170) {
		pos -= 85;
		return ((uint32_t) (0) << 16) | ((uint32_t) (pos * 3) << 8)
				| (255 - pos * 3);
	} else {
		pos -= 170;
		return ((uint32_t) (pos * 3) << 16)
				| ((uint32_t) (_intensity - pos * 3) << 8) | (0);
	}
}

uint16_t offset = 0;

void colour_scroll(ws2812_rgb_struct *_rgb_struct) {

	UINT32_RGB _rgb = { { 0, 0, 0, 0 } };
	uint16_t _pos = offset;
	uint32_t temp;
	for (uint8_t i = 0; i < NUMBER_OF_PIXELS; ++i) {
		_pos < 0xFF ? ++_pos : 0;
		temp = (color_wheel(_pos, 64));
		_rgb.xUINT = temp;
		set_pixel_GRB(_rgb_struct, &_rgb, i);

	}

	shift_offset();
}

void hsv_scroll(ws2812_rgb_struct *_rgb_struct) {

	int n = 256; // number of steps
	float TWO_PI = 3.14159 * 2;
	UINT32_RGB _rgb = { { 0, 0, 0, 0 } };

	for (uint16_t i = 0; i < NUMBER_OF_PIXELS; ++i) {
		_rgb.xRGB.red = 8 + sin((i + offset) * TWO_PI / n + 0) + 8;
		_rgb.xRGB.green = 8 + sin((i + offset) * TWO_PI / n + TWO_PI / 3) + 8;
		_rgb.xRGB.blue = 8 + sin((i + offset) * TWO_PI / n + 2 * TWO_PI / 8)
				+ 8;
		set_pixel_GRB(_rgb_struct, &_rgb, i);
	}
	shift_offset();

}

void shift_offset()
{
	if (offset < 256) {
		++offset;
	} else {
		offset = 0;
	}
}

