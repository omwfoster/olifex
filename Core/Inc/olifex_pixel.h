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



#define WORDS_PER_PIXEL				24
#define ZERO_PADDING 				300
#define BUFFER_LENGTH 				((NUMBER_OF_PIXELS * WORDS_PER_PIXEL)  + ZERO_PADDING)



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

typedef struct xHSV {
	uint16_t hue;
	uint16_t sat;
	uint16_t val;
	uint16_t alpha;
}XHSV;


typedef union uint32_colour
{
	XRGB	  xRGB;
	uint32_t  xUINT;
	XHSV	  xHSV;
} UCOL;





typedef struct {
	uint16_t length;
	UCOL *ptr_start;
	UCOL *ptr_end;
	UCOL *cursor;
	UCOL *cursor_up;
	UCOL *cursor_down;
	enum {
		UP,DOWN
	} frame_direction;


} ws2812_rgb_struct;



void       write_frame_to_output(ws2812_rgb_struct *,pwm_output_struct *);
void       shift_frame(ws2812_rgb_struct * _ws_struct,uint16_t _magnitude );
void       set_pixel_GRB(ws2812_rgb_struct *_ws_struct,UCOL *_Color,uint32_t _loc);
uint16_t   calc_GCD(uint16_t a,uint16_t b);
UCOL hsv2rgb(XHSV *_Color_HSV);







#endif /* INC_PIXEL_H_ */
