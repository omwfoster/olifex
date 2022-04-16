/*

 */
#include "olifex_fx.h"

fx_config  * fx_cfg1;
bool fx_init = false;

dispatch_table  * fx_array_struct;




float mapf(float value, float c_min, float c_max, float t_min, float t_max) {
    return (value - c_min) / (c_max - c_min) * (t_max - t_min) + t_min;
}

float bound(float value, float max, float min) {
    return fmaxf(fminf(value, max), min);
}




void init_effects(fx_config *p_fx) {

	fx_init = true;
	fx_cfg1 = p_fx;
	p_fx->hue_offset = 0;
	p_fx->sat_offset = 0;
	p_fx->val_offset = 0;
	p_fx->pos_offset = 0;
	p_fx->number_pixels = NUMBER_OF_PIXELS;
	p_fx->direction = true;



	dispatch_table dt1 = {4 ,
							{
									{rgb_scroll , "rgb_scrl"},
									{hsv_scroll , "hsv_scrl"},
									{hsv_wave ,   "hsv_wave"},
									{fire_fill ,  "fire_fll"}
							}
						  };



	fx_array_struct = &dt1;

}

void shift_hue(fx_config *p_fx) {
	if (p_fx->hue_offset < 3600) {
		p_fx->hue_offset++;
	} else {
		p_fx->hue_offset = 0;
	}
}

void shift_sat(fx_config *p_fx) {
	if (p_fx->sat_offset < 1000) {
		p_fx->sat_offset++;
	} else {
		p_fx->sat_offset = 0;
	}
}

void shift_val(fx_config *p_fx) {
	if (p_fx->val_offset < 1000) {
		p_fx->val_offset++;
	} else {
		p_fx->val_offset = 0;
	}
}

void shift_pos(fx_config *p_fx) {
	if (p_fx->pos_offset < 0xFFFF) {
		p_fx->pos_offset++;
	} else {
		p_fx->pos_offset = 0;
	}
}



/*
 * Put a value 0 to 255 in to get  a color value.
 * The colours are a transition r -> g -> b -> back to r
 * Inspired by the Adafruit examples.
 */

uint32_t color_wheel(uint16_t pos, uint16_t _intensity) {
	pos = 255 - pos;
	if (pos < 85) {
		return ((uint32_t) (_intensity - pos * 3) << 16) | ((uint32_t) (0) << 8)
				| (pos * 3);
	} else if (pos < 170) {
		pos -= 85;
		return ((uint32_t) (0) << 16) | ((uint32_t) (pos * 3) << 8)
				| (255 - pos * 3);
	} else {
		pos -= 170;
		return ((uint32_t) (pos * 3) << 16)
				| ((uint32_t) (_intensity - pos * 3) << 8) | (0);
	}
}

void rgb_scroll(ws2812_rgb_struct *rgb_struct) {

	UINT32_RGB rgb = { { 0, 0, 0, 0 } };

	uint32_t temp;
	for (uint8_t i = 0; i < NUMBER_OF_PIXELS; ++i) {
		fx_cfg1->pos_offset < 0xFF ? fx_cfg1->pos_offset++ : 0;
		temp = (color_wheel(fx_cfg1->pos_offset, 64));
		rgb.xUINT = temp;
		set_pixel_GRB(rgb_struct, &rgb, i);
	}

	shift_pos(fx_cfg1);

}

void hsv_scroll(ws2812_rgb_struct *_rgb_struct) {

	struct_HSV _hsv = { fx_cfg1->hue_offset, 1000, 50, 0 };
	UINT32_RGB _rgb = { { 0, 0, 0, 0 } };

	for (uint16_t i = 0; i < NUMBER_OF_PIXELS; ++i) {
		_hsv.hue++;
		_rgb = hsv2rgb(&_hsv);
		set_pixel_GRB(_rgb_struct, &_rgb, map_to_pixel(i));
	}

	shift_hue(fx_cfg1);

}

void hsv_wave(ws2812_rgb_struct *_rgb_struct) {

	struct_HSV _hsv = { fx_cfg1->hue_offset, 1000, 50, 0 };
	UINT32_RGB _rgb = { { 0, 0, 0, 0 } };
	uint8_t i_sin = 0;

	for (uint16_t i = 0; i < NUMBER_OF_PIXELS; ++i) {
		_hsv.hue++;
		_hsv.val = (((uint16_t) sine_wave[i_sin] / 16));
		_rgb = hsv2rgb(&_hsv);
		set_pixel_GRB(_rgb_struct, &_rgb, map_to_pixel(i));
		i_sin < 255 ? i_sin++ : 0;
	}

	shift_hue(fx_cfg1);

}




void rgb_wave(ws2812_rgb_struct *rgb_struct) {
    static float time;
    float xx;
    UINT32_RGB _rgb = { { 0, 0, 0, 0 } };


    if(time > 2*M_PI) {
        time = 0.0;
    }

    static uint16_t index = 0;
    for(uint8_t y = 0; y < PIXEL_ROWS; y++) {
        for(uint8_t x = 0; x < PIXEL_COLUMNS; x++) {
            xx = mapf(x, 0, PIXEL_COLUMNS-1, 0, 2*M_PI);
            _rgb.xRGB.red = 16 + 100 * (bound(sinf(xx + time + 2*M_PI/3), 0.5, -0.5) + 0.5);
            _rgb.xRGB.green = 16 + 100 * (bound(sinf(xx + time - 2*M_PI/3), 0.5, -0.5) + 0.5);
            _rgb.xRGB.blue = 16 + 100 * (bound(sinf(xx + time         ), 0.5, -0.5) + 0.5);
            set_pixel_GRB(rgb_struct, &_rgb, map_to_pixel(index));
            index++;
        }
    }
    time += 0.1;
}

void plasma_fill(ws2812_rgb_struct *rgb_struct) {
    static float time;
    float xx, yy;
    float v;
    float delta = 0.025;

    time += 0.025;
    if(time > 12*M_PI) {
        delta *= -1;
    }

    for(uint8_t y = 0; y < PIXEL_ROWS; y++) {
        yy = mapf(y, 0, PIXEL_ROWS-1, 0, 2*M_PI);
        for(uint8_t x = 0; x < PIXEL_COLUMNS; x++) {
            xx = mapf(x, 0, PIXEL_COLUMNS-1, 0, 2*M_PI);

            v = sinf(xx + time);
            v += sinf((yy + time) / 2.0);
            v += sinf((xx + yy + time) / 2.0);
            float cx = xx + .5 * sinf(time/5.0);
            float cy = yy + .5 * cosf(time/3.0);
            v += sinf(sqrtf((cx*cx+cy*cy)+1)+time);
            v /= 2.0;
     //       _rgb.xRGB.red = 255 * (sinf(v * M_PI) + 1) / 2;
     //       _rgb.xRGB.green = 255 * (cosf(v * M_PI) + 1) / 2;
     //       _rgb.xRGB.blue = 128 * (sinf(v * M_PI + 2*M_PI/3) + 1) / 2;
       //     PIXEL(frame, x, y).R = r;
       //     PIXEL(frame, x, y).G = g;
       //     PIXEL(frame, x, y).B = b;
        }
    }
}




#define ROW_BUFFER_LENGTH PIXEL_COLUMNS+2
static UINT32_RGB  rgb_row_buffer[ROW_BUFFER_LENGTH];


void fire_fill(ws2812_rgb_struct *_ws_struct) {


	static uint8_t heat[NUMBER_OF_PIXELS];
	memset(&rgb_row_buffer[0],0,(ROW_BUFFER_LENGTH * sizeof(UINT32_RGB)));


	for (uint8_t i = 0; i < (NUMBER_OF_PIXELS - 2); ++i) {
		rgb_row_buffer[i].xUINT = ((_ws_struct->cursor[i].xUINT + _ws_struct->cursor[i+1].xUINT + _ws_struct->cursor[i+2].xUINT));

	}

	// Step 2.  Heat from each cell drifts 'up' and diffuses a little
	for (uint16_t k = 0; k >= (PIXEL_ROWS-1); k--) {
		for (uint8_t i = 0; i < (NUMBER_OF_PIXELS - 2); ++i)
		{
			rgb_row_buffer[i].xUINT = ((_ws_struct->cursor[i].xUINT + _ws_struct->cursor[i+1].xUINT + _ws_struct->cursor[i+2].xUINT));
		}
	}

	// Step 3.  Randomly ignite new 'sparks' of heat near the bottom
	for (uint8_t i = 0; i < (PIXEL_COLUMNS - 1); ++i) {
		rgb_row_buffer[i].xUINT = ((_ws_struct->cursor[i].xUINT + _ws_struct->cursor[i+1].xUINT + _ws_struct->cursor[i+2].xUINT));

	}


	// Step 4.  Map from heat cells to LED colors
	for (uint8_t i = 0; i < (NUMBER_OF_PIXELS - 2); ++i)
	{
		set_pixel_GRB(&_ws_struct->cursor[i], &rgb_row_buffer[0], map_to_pixel(i));
	}

}

void heat_color(UINT32_RGB * _rgb, uint8_t temperature)
{
    // Scale 'heat' down from 0-255 to 0-191
    uint8_t t192 = round((temperature / 255.0) * 191);

    // calculate ramp up from
    uint8_t heatramp = t192 & 0x3F; // 0..63
    heatramp <<= 2;                 // scale up to 0..252

    // figure out which third of the spectrum we're in:
    if (t192 > 0x80)
    { // hottest
         _rgb->xRGB.red = 255;
         _rgb->xRGB.green = 255;
         _rgb->xRGB.blue = heatramp;
    }
    else if (t192 > 0x40)
    { // middle
    	_rgb->xRGB.red = 255;
    	_rgb->xRGB.green = heatramp;
    	_rgb->xRGB.blue = 0;
    }
    else
    { // coolest
    	_rgb->xRGB.red = heatramp;
    	_rgb->xRGB.green = 0;
    	_rgb->xRGB.blue = 0;
    }

}

void setColorBrightness(const UINT32_RGB * in, UINT32_RGB * out, float brightness)
{
    out->xRGB.red   = brightness * in->xRGB.red;
    out->xRGB.green = brightness * in->xRGB.green;
    out->xRGB.blue  = brightness * in->xRGB.blue;
}

void RunningLights(UINT32_RGB * c, uint32_t delay_ms, uint32_t time_s)
{
	static UINT32_RGB new_color;
    int num_of_frames = time_s * 1000 / delay_ms;
    for (int j = 0; j < num_of_frames; j++)
    {
        for (uint16_t i = 0; i < NUMBER_OF_PIXELS; i++)
        {
        //    setColorBrightness(c,&new_color,(sin(i + j) * 127 + 128) / 255.0);
       //     setPixel_GRB(&new_color, i);
        }

    }
}
