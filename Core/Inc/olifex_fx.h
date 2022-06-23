/*
 * effects.h
 *
 *  Created on: Jan 7, 2022
 *      Author: omwfo
 */

#ifndef INC_EFFECTS_H_
#define INC_EFFECTS_H_

#include <olifex_pixel.h>
#include "tables.h"
#include "stdbool.h"
#include "string.h"



#define DIM(c)     (uint32_t)((c >> 2) & 0x3f3f3f3f) // color at 25% intensity
#define DARK(c)    (uint32_t)((c >> 4) & 0x0f0f0f0f) // color at  6% intensity



#define CLAMP(a,b) \
    ({ \
        const typeof (a) _a = (a); \
        const typeof (b) _b = (b); \
        (_a > _b ? _a : _b); \
    })


typedef struct __attribute__((packed, aligned(4)))  fx_polar_coord{
	q15_t mag;
	q15_t theta;
}fx_polar_coord;

typedef struct __attribute__((packed, aligned(4)))  fx_vector_coord{
	q15_t x;
	q15_t y;
}fx_vec_coord;


typedef struct __attribute__((packed, aligned(4)))  fx_grad_cells{

	uint16_t cell_size_x;   // cell size x
	uint16_t cell_size_y;   // cell size y
	uint16_t row_offset;
	uint16_t col_offset;
	uint16_t cells_x;
	uint16_t cells_y;
	uint16_t n_vectors;
	fx_polar_coord   * grad_vectors;

}fx_cells;

typedef struct fx_config {

	uint16_t 	n_pixels;
	uint16_t 	row_len; //pixels per row
	uint16_t 	col_len; //pixels per column
	bool	    direction;
	fx_cells    grad_cells;
	uint16_t  * map_xy;
	UCOL      * pixel_array;
	uint16_t 	hue_offset;
	uint16_t 	sat_offset;
	uint16_t 	val_offset;
	uint16_t 	pos_offset;

}fx_config;



void init_olifex_fx(fx_config *p_fx,uint16_t col_len,uint16_t row_len,uint16_t cell_size);



uint32_t color_wheel(uint16_t pos,uint16_t _intensity);
void rgb_scroll(ws2812_rgb_struct *_ws_struct, fx_config * p_fx);
void hsv_scroll(ws2812_rgb_struct *_ws_struct, fx_config * p_fx);
void hsv_wave(ws2812_rgb_struct  *_ws_struct, fx_config * p_fx);
void fire_fill(ws2812_rgb_struct *_ws_struct,fx_config * p_fx);
void perlin(ws2812_rgb_struct *_ws_struct, fx_config * p_fx);
void heat_color(UCOL * _rgb, uint8_t temperature);
void shift_offset();
uint8_t    fill_pixel_map(fx_config * p_fx);
uint16_t   map_to_pixel(uint16_t i, fx_config * p_fx);


#endif /* INC_EFFECTS_H_ */
