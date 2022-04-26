/*
 * effects.h
 *
 *  Created on: Jan 7, 2022
 *      Author: omwfo
 */

#ifndef INC_EFFECTS_H_
#define INC_EFFECTS_H_

#include <olifex_pixel.h>
#include "math.h"
#include <arm_math.h>
#include "tables.h"
#include "stdbool.h"
#include "string.h"

#define FX_TABLE_SIZE 4

#define CLAMP(a,b) \
    ({ \
        const typeof (a) _a = (a); \
        const typeof (b) _b = (b); \
        (_a > _b ? _a : _b); \
    })




typedef struct fx_config {
	uint16_t hue_offset;
	uint16_t sat_offset;
	uint16_t val_offset;
	uint16_t pos_offset;
	uint16_t number_pixels;
	uint16_t number_rows;
	uint16_t number_columns;
	bool	 direction;
	float32_t * grad_vectors;

}fx_config;





#define DIM(c)     (uint32_t)((c >> 2) & 0x3f3f3f3f) // color at 25% intensity
#define DARK(c)    (uint32_t)((c >> 4) & 0x0f0f0f0f) // color at  6% intensity



void init_fx(fx_config *p_fx);


uint32_t color_wheel(uint16_t pos,uint16_t _intensity);
void rgb_scroll(ws2812_rgb_struct *_ws_struct, fx_config * p_fx);
void hsv_scroll(ws2812_rgb_struct *_ws_struct, fx_config * p_fx);
void hsv_wave(ws2812_rgb_struct  *_ws_struct, fx_config * p_fx);
void fire_fill(ws2812_rgb_struct *_ws_struct,fx_config * p_fx);
void perlin(ws2812_rgb_struct *_ws_struct, fx_config * p_fx);
void heat_color(UINT32_RGB * _rgb, uint8_t temperature);
void shift_offset();




#endif /* INC_EFFECTS_H_ */
