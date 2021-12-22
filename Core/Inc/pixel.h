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

#define NUMBER_OF_PIXELS    	    1
#define WORDS_PER_PIXEL				24
#define ZERO_PADDING 				52
#define BUFFER_LENGTH 				((NUMBER_OF_PIXELS * WORDS_PER_PIXEL)  + ZERO_PADDING)

typedef struct {
	uint16_t *output_buffer;
	uint16_t *ptr_start;
	uint16_t *ptr_end;
	uint16_t *cursor;
	enum {
		LOCKED, WAITING
	} frame_status;



} pwm_output_struct;


typedef struct xRGB {
	uint8_t unused;
	uint8_t blue;
	uint8_t red;
	uint8_t green;
}XRGB;


typedef union uint32_RGB
{
	XRGB	  xRGB;
	uint32_t  xUINT;
} UINT32_RGB;

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




void write_frame_to_output(ws2812_rgb_struct *,pwm_output_struct *);
void write_pixel_to_output(uint32_t *color, uint16_t *cursor);
void shift_frame(ws2812_rgb_struct * _ws_struct,uint16_t _magnitude );
uint16_t calc_GCD(uint16_t a,uint16_t b);
void set_color_GRB(XRGB *_Color, UINT32_RGB *_Pixel,uint32_t _len);
void set_pixel_GRB(ws2812_rgb_struct *_ws_struct,UINT32_RGB *_Color,uint32_t _loc);
void write_output_buffer(uint32_t *color, uint16_t *cursor, uint16_t _len);




////////////////////////////////////////////////////////////////////////
void clearAll(void);
void getPixelColor(int px_index, UINT32_RGB * px_color);
void setRange_GRB(const UINT32_RGB * color, int start_px_index, int len);
void setColorBrightness(const UINT32_RGB * in, UINT32_RGB * out, float brightness);
////////////////////////////////////////////////////////////////////////





#endif /* INC_PIXEL_H_ */
