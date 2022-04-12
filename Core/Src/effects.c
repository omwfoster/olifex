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

uint32_t color_wheel(uint16_t pos) {
	pos = 255 - pos;
	if (pos < 85) {
		return ((uint32_t) (255 - pos * 3) << 16) | ((uint32_t) (0) << 8)
				| (pos * 3);
	} else if (pos < 170) {
		pos -= 85;
		return ((uint32_t) (0) << 16) | ((uint32_t) (pos * 3) << 8)
				| (255 - pos * 3);
	} else {
		pos -= 170;
		return ((uint32_t) (pos * 3) << 16) | ((uint32_t) (255 - pos * 3) << 8)
				| (0);
	}
}
