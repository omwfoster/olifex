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
#include <arm_math.h>





float TWO_PI = 3.14159 * 2;


void init_olifex_pixel(uint16_t rows,uint16_t columns)
{

}

void blend( UCOL * colourA,UCOL * colourB, UCOL *colourOut,uint16_t palette_offset) {
	float32_t v =   (float32_t)palette_offset/(float32_t)MAX_UINT16;

	uint8_t r = ((float32_t) colourB->xRGB.red * v) + ((float32_t) colourB->xRGB.red * (1.0 - v));
	uint8_t g = ((float32_t) colourB->xRGB.green * v) + ((float32_t) colourB->xRGB.green * (1.0 - v));
	uint8_t b = ((float32_t) colourB->xRGB.blue * v) + ((float32_t) colourB->xRGB.blue * (1.0 - v));

	colourOut->xRGB.red = (r > 255.0) ? 255.0 : (r < 0.0) ? 0.0 : r;
	colourOut->xRGB.green = (g > 255.0) ? 255.0 : (g < 0.0) ? 0.0 : g;
	colourOut->xRGB.blue = (b > 255.0) ? 255.0 : (b < 0.0) ? 0.0 : b;
}



void set_pixel_GRB(ws2812_rgb_struct *_ws_struct, UCOL *_Color,
		uint32_t _pos) {

	_ws_struct->cursor    =   _ws_struct->ptr_start;
	_ws_struct->cursor   +=   _pos;
	* _ws_struct->cursor  = * _Color;

}

UCOL hsv2rgb(XHSV *_Color_HSV)
{

	UCOL _Color_RGB;

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
	UCOL _rgb;

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
void shift_frame(ws2812_rgb_struct *_ws_struct, uint16_t stride) {

	UCOL _xrgb;

	if (_ws_struct->length <= 1 || !stride)
		return;
	stride = stride % _ws_struct->length;

	uint16_t i, j, k, _gcd = calc_GCD(_ws_struct->length, stride);

	for (i = 0; i < _gcd; i++) {
		_xrgb = _ws_struct->ptr_start[i];
		for (j = i; 1; j = k) {
			k = j + stride;
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

uint16_t  get_start(uint16_t cell_x,uint16_t cell_y,uint16_t stride,uint16_t depth)
{

	return ((cell_x * stride)+(cell_y * depth));

}









