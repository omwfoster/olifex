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
#include "tables.h"
#include "stdbool.h"
#include "string.h"



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
	bool	 direction;

}fx_config;

extern fx_config  fx_cfg1;

typedef void (*fx_fp)(ws2812_rgb_struct *rgb_struct);

typedef struct fx_struct{
	fx_fp fp;
	char fx_name[8];
}dispatch_entry;


typedef struct dispatch_table{
	uint8_t n_fx;
	dispatch_entry fx_struct[];
}dispatch_table;





#define DIM(c)     (uint32_t)((c >> 2) & 0x3f3f3f3f) // color at 25% intensity
#define DARK(c)    (uint32_t)((c >> 4) & 0x0f0f0f0f) // color at  6% intensity



void init_effects(fx_config * p_fx);


uint32_t color_wheel(uint16_t pos,uint16_t _intensity);
void rgb_scroll(ws2812_rgb_struct *_rgb_struct);
void hsv_scroll(ws2812_rgb_struct *_rgb_struct);
void hsv_wave(ws2812_rgb_struct *_ws_struct);
void fire_fill(ws2812_rgb_struct *_ws_struct);
void heat_color(UINT32_RGB * _rgb, uint8_t temperature);
void shift_offset();




#endif /* INC_EFFECTS_H_ */
