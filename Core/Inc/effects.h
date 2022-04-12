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

typedef struct Xanimate {
	uint16_t hue_offset;
	uint16_t sat_offset;
	uint16_t val_offset;
	uint16_t pos_offset;
	uint16_t number_pixels;
	bool	 direction;

}animate_typedef;

extern animate_typedef  anim1;

typedef void (*func)(ws2812_rgb_struct *rgb_struct);

typedef struct dispatch_entry{
	func fp,
	char name[8]
}dispatch_entry;


typedef struct dispatch_table{
	uint8_t n_fx,
	dispatch_entry ptr_fx
};





#define DIM(c)     (uint32_t)((c >> 2) & 0x3f3f3f3f) // color at 25% intensity
#define DARK(c)    (uint32_t)((c >> 4) & 0x0f0f0f0f) // color at  6% intensity



void init_effects(animate_typedef * ani);


uint32_t color_wheel(uint16_t pos,uint16_t _intensity);
void rgb_scroll(ws2812_rgb_struct *_rgb_struct);
void hsv_scroll(ws2812_rgb_struct *_rgb_struct);
void hsv_wave(ws2812_rgb_struct *_ws_struct);
void fire_fill(ws2812_rgb_struct *_ws_struct);
void heat_color(UINT32_RGB * _rgb, uint8_t temperature);
void shift_offset();




#endif /* INC_EFFECTS_H_ */
