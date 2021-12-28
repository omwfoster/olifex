/*
 * pixel.c
 *
 *  Created on: Nov 4, 2021
 *      Author: omwfo
 */

#include "pixel.h"
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

#define RAM_BASE 0x20000000
#define RAM_BB_BASE 0x22000000
#define Var_ResetBit_BB(VarAddr, BitNumber) (*(volatile uint32_t *) (RAM_BB_BASE | ((VarAddr - RAM_BASE) << 5) | ((BitNumber) << 2)) = 0)
#define Var_SetBit_BB(VarAddr, BitNumber) (*(volatile uint32_t *) (RAM_BB_BASE | ((VarAddr - RAM_BASE) << 5) | ((BitNumber) << 2)) = 1)
#define Var_GetBit_BB(VarAddr, BitNumber) (*(volatile uint32_t *) (RAM_BB_BASE | ((VarAddr - RAM_BASE) << 5) | ((BitNumber) << 2)))
#define BITBAND_SRAM(address, bit) ( (__IO uint32_t *) (RAM_BB_BASE + (((uint32_t)address) - RAM_BASE) * 32 + (bit) * 4))

#define varSetBit(var,bit) (Var_SetBit_BB((uint32_t)&var,bit))
#define varResetBit(var,bit) (Var_ResetBit_BB((uint32_t)&var,bit))
#define varGetBit(var,bit) (Var_GetBit_BB((uint32_t)&var,bit))

#define TIM_COMPARE_HIGH	31
#define TIM_COMPARE_LOW		70

void blend(const uint8_t *colourA, const uint8_t *colourB, uint8_t *colourOut,
		float amount) {
	float r, g, b;

	r = ((float) colourB[0] * amount) + ((float) colourA[0] * (1.0 - amount));
	g = ((float) colourB[1] * amount) + ((float) colourA[1] * (1.0 - amount));
	b = ((float) colourB[2] * amount) + ((float) colourA[2] * (1.0 - amount));

	colourOut[0] = (r > 255.0) ? 255.0 : (r < 0.0) ? 0.0 : r;
	colourOut[1] = (g > 255.0) ? 255.0 : (g < 0.0) ? 0.0 : g;
	colourOut[2] = (b > 255.0) ? 255.0 : (b < 0.0) ? 0.0 : b;
}

void set_color_GRB(XRGB *_Color, UINT32_RGB *_Pixel, uint32_t _len) {
	static uint32_t i;
	for (i = 1; i <= _len; ++i) {
		_Pixel->xRGB = *_Color;
		++_Pixel;
	}
}

void set_pixel_GRB(ws2812_rgb_struct *_ws_struct, UINT32_RGB *_Color,
		uint32_t _loc) {

	if ((_ws_struct->length) > _loc) {
		_ws_struct->ptr_start[_loc] = *_Color;
	}

}

void write_pixel_to_output(uint32_t *color, uint16_t *cursor) {

	uint16_t *_cursor = cursor;
	uint16_t i;

	for (i = 32; i >= 8; --i) {
		*_cursor =
				(varGetBit((*color), (i))) ? TIM_COMPARE_LOW : TIM_COMPARE_HIGH;
		_cursor++;

	}

}

void write_frame_to_output(ws2812_rgb_struct *_rgb_struct,
		pwm_output_struct *_pwm_struct) {

	_rgb_struct->cursor = _rgb_struct->ptr_start;
	_pwm_struct->cursor = _pwm_struct->ptr_start;

	while (_rgb_struct->cursor < (_rgb_struct->ptr_end)) {

		for (uint8_t i = 32; i >= 8; --i) {
			if (_pwm_struct->cursor < _pwm_struct->ptr_end) {
				*_pwm_struct->cursor =
						(varGetBit((_rgb_struct->cursor), (i))|1U) ?
								TIM_COMPARE_LOW : TIM_COMPARE_HIGH;
				_pwm_struct->cursor++;
			}
		}
		_rgb_struct->cursor++;
	}

	_rgb_struct->cursor = _rgb_struct->ptr_start;
	_pwm_struct->cursor = _pwm_struct->ptr_start;

}
void shift_frame(ws2812_rgb_struct *_ws_struct, uint16_t _magnitude) {

	UINT32_RGB _xrgb;

	if (_ws_struct->length <= 1 || !_magnitude)
		return;
	_magnitude = _magnitude % _ws_struct->length;

	uint16_t i, j, k, _gcd = calc_GCD(_ws_struct->length, _magnitude);

	for (i = 0; i < _gcd; i++) {
		_xrgb = _ws_struct->ptr_start[i];
		for (j = i; 1; j = k) {
			k = j + _magnitude;
			if (k >= _ws_struct->length)
				k -= _ws_struct->length;
			if (k == i)
				break;

			_ws_struct->ptr_start[j] = _ws_struct->ptr_start[k];

		}
		_ws_struct->ptr_start[j] = _xrgb;
	}

	_ws_struct->cursor = _ws_struct->ptr_start;

}

// Get Greatest Common Divisor using binary GCD algorithm
// http://en.wikipedia.org/wiki/Binary_GCD_algorithm
uint16_t calc_GCD(uint16_t a, uint16_t b) {
	uint16_t shift, tmp;

	if (a == 0)
		return b;
	if (b == 0)
		return a;

	// Find power of two divisor
	for (shift = 0; ((a | b) & 1) == 0; shift++) {
		a >>= 1;
		b >>= 1;
	}

	// Remove remaining factors of two from a - they are not common
	while ((a & 1) == 0)
		a >>= 1;

	do {
		// Remove remaining factors of two from b - they are not common
		while ((b & 1) == 0)
			b >>= 1;

		if (a > b) {
			tmp = a;
			a = b;
			b = tmp;
		} // swap a,b
		b = b - a;
	} while (b != 0);

	return a << shift;
}

