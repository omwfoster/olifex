/*
 * pixel.c
 *
 *  Created on: Nov 4, 2021
 *      Author: omwfo
 */

#include "olifex_pixel.h"
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>





float TWO_PI = 3.14159 * 2;


void init_olifex_pixel(uint16_t rows,uint16_t columns)
{

}

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



void set_pixel_GRB(ws2812_rgb_struct *_ws_struct, UINT32_RGB *_Color,
		uint32_t _pos) {

	_ws_struct->cursor    =   _ws_struct->ptr_start;
	_ws_struct->cursor   +=   _pos;
	* _ws_struct->cursor  = * _Color;

}

UINT32_RGB hsv2rgb(struct_HSV *_Color_HSV)
{

	UINT32_RGB _Color_RGB;

	_Color_RGB.xRGB.red = 0 ;
	_Color_RGB.xRGB.green = 0 ;
	_Color_RGB.xRGB.blue = 0 ;


	    if (_Color_HSV->hue == 0)
	    {
	    	_Color_RGB.xRGB.red = (uint8_t)((255 * _Color_HSV->val) / 1000);
	    	_Color_RGB.xRGB.green = _Color_RGB.xRGB.red;
	    	_Color_RGB.xRGB.blue  = _Color_RGB.xRGB.red;

	    }
	    else
	    {
	        int16_t h = _Color_HSV->hue /600;
	        int16_t f = ((_Color_HSV->hue %600)*1000)/600;
	        int16_t p = (_Color_HSV->val *(1000-_Color_HSV->sat))/1000;
	        int16_t q = (_Color_HSV->val*(1000-((_Color_HSV->sat*f)/1000)))/1000;
	        int16_t t = (_Color_HSV->val*(1000-((_Color_HSV->sat*(1000-f))/1000)))/1000;

	        switch (h)
	        {
	        case 0:

	        	_Color_RGB.xRGB.red = (uint8_t)((255 * _Color_HSV->val) / 1000);
	        	_Color_RGB.xRGB.green = (uint8_t)((255 * t) / 1000);
	        	_Color_RGB.xRGB.blue = (uint8_t)((255 * p) / 1000);
	            break;

	        case 1:

	        	_Color_RGB.xRGB.red = (uint8_t)((255 * q) / 1000);
	        	_Color_RGB.xRGB.green = (uint8_t)((255 * _Color_HSV->val) / 1000);
	        	_Color_RGB.xRGB.blue = (uint8_t)((255 * p) / 1000);
	            break;

	        case 2:

	        	_Color_RGB.xRGB.red = (uint8_t)((255 * p) / 1000);
	        	_Color_RGB.xRGB.green = (uint8_t)((255 * _Color_HSV->val) / 1000);
	        	_Color_RGB.xRGB.blue = (uint8_t)((255 * t) / 1000);
	            break;

	        case 3:

	        	_Color_RGB.xRGB.red = (uint8_t)((255 * p) / 1000);
	        	_Color_RGB.xRGB.green = (uint8_t)((255 * q) / 1000);
	        	_Color_RGB.xRGB.blue = (uint8_t)((255 * _Color_HSV->val) / 1000);
	            break;

	        case 4:

	        	_Color_RGB.xRGB.red = (uint8_t)((255 * t) / 1000);
	        	_Color_RGB.xRGB.green = (uint8_t)((255 * p) / 1000);
	        	_Color_RGB.xRGB.blue = (uint8_t)((255 * _Color_HSV->val) / 1000);
	            break;

	        case 5:

	        	_Color_RGB.xRGB.red = (uint8_t)((255 * _Color_HSV->val) / 1000);
	        	_Color_RGB.xRGB.green = (uint8_t)((255 * p) / 1000);
	            _Color_RGB.xRGB.blue = (uint8_t)((255 * q) / 1000);
	            break;

	        }
	    }
	    return _Color_RGB;

}

void write_frame_to_output(ws2812_rgb_struct *_rgb_struct,
		pwm_output_struct *_pwm_struct) {

	_rgb_struct->cursor = _rgb_struct->ptr_start;
	_pwm_struct->cursor = _pwm_struct->ptr_start;
	UINT32_RGB _rgb;

	while (_rgb_struct->cursor <= (_rgb_struct->ptr_end)) {

		for (uint8_t i = 0; i <= 23; ++i) {
			if (_pwm_struct->cursor < _pwm_struct->ptr_end) {
				_rgb = *_rgb_struct->cursor;
				*_pwm_struct->cursor  =  (_rgb.xUINT  & (1<<i))?
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









