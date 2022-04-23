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


#define TIM_PERIOD			52
#define TIM_COMPARE_HIGH	16
#define TIM_COMPARE_LOW		35

#define NUMBER_OF_PIXELS    	    40
#define PIXEL_ROWS					5
#define PIXEL_COLUMNS				8

#define WORDS_PER_PIXEL				24
#define ZERO_PADDING 				300
#define BUFFER_LENGTH 				((NUMBER_OF_PIXELS * WORDS_PER_PIXEL)  + ZERO_PADDING)


extern uint16_t map_xy[];

typedef struct {
	uint16_t *output_buffer;
	uint16_t *ptr_start;
	uint16_t *ptr_end;
	uint16_t length;
	uint16_t *cursor;
	enum {
		LOCKED, WAITING
	} frame_status;

} pwm_output_struct;


typedef struct xRGB {
	uint8_t blue;
	uint8_t red;
	uint8_t green;
	uint8_t alpha;
}XRGB;


typedef union uint32_RGB
{
	XRGB	  xRGB;
	uint32_t  xUINT;
} UINT32_RGB;


typedef struct xHSV {
	uint16_t hue;
	uint16_t sat;
	uint16_t val;
	uint16_t alpha;
}struct_HSV;


typedef struct {
	uint16_t length;
	UINT32_RGB *ptr_start;
	UINT32_RGB *ptr_end;
	UINT32_RGB *cursor;
	UINT32_RGB *cursor_up;
	UINT32_RGB *cursor_down;
	enum {
		UP,DOWN
	} frame_direction;


} ws2812_rgb_struct;



void       write_frame_to_output(ws2812_rgb_struct *,pwm_output_struct *);
void       shift_frame(ws2812_rgb_struct * _ws_struct,uint16_t _magnitude );
void       set_pixel_GRB(ws2812_rgb_struct *_ws_struct,UINT32_RGB *_Color,uint32_t _loc);
uint16_t   calc_GCD(uint16_t a,uint16_t b);
UINT32_RGB hsv2rgb(struct_HSV *_Color_HSV);
uint8_t    fill_pixel_map();
uint16_t   map_to_pixel(uint16_t i);






#endif /* INC_PIXEL_H_ */
