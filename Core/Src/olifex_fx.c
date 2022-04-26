/*

 */
#include "olifex_fx.h"
#include "fastnoise.h"
#include "stdlib.h"

static fx_config  * fx_cfg1;
bool fx_init = false;









float mapf(float value, float c_min, float c_max, float t_min, float t_max) {
    return (value - c_min) / (c_max - c_min) * (t_max - t_min) + t_min;
}

float bound(float value, float max, float min) {
    return fmaxf(fminf(value, max), min);
}




void init_fx(fx_config *p_fx) {

	fx_init = true;
	fx_cfg1 = p_fx;
	p_fx->hue_offset = 0;
	p_fx->sat_offset = 0;
	p_fx->val_offset = 0;
	p_fx->pos_offset = 0;
//	p_fx->number_pixels = NUMBER_OF_PIXELS;
	p_fx->direction = true;

	p_fx->grad_vectors = malloc(p_fx->number_pixels);
	memcpy(p_fx->grad_vectors,RAND_VECS_2D,((PIXEL_ROWS+1)*(PIXEL_COLUMNS+1))*(sizeof(float32_t)));





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

void rgb_scroll(ws2812_rgb_struct *rgb_struct, fx_config * p_fx) {

	UINT32_RGB rgb = { { 0, 0, 0, 0 } };

	uint32_t temp;
	for (uint8_t i = 0; i < p_fx->number_pixels; ++i) {
		fx_cfg1->pos_offset < 0xFF ? fx_cfg1->pos_offset++ : 0;
		temp = (color_wheel(fx_cfg1->pos_offset, 64));
		rgb.xUINT = temp;
		set_pixel_GRB(rgb_struct, &rgb, i);
	}

	shift_pos(fx_cfg1);

}

void hsv_scroll(ws2812_rgb_struct *rgb_struct, fx_config * p_fx) {

	struct_HSV _hsv = { fx_cfg1->hue_offset, 1000, 50, 0 };
	UINT32_RGB _rgb = { { 0, 0, 0, 0 } };

	for (uint16_t i = 0; i < p_fx->number_pixels; ++i) {
		_hsv.hue++;
		_rgb = hsv2rgb(&_hsv);
		set_pixel_GRB(rgb_struct, &_rgb, map_to_pixel(i));
	}

	shift_hue(fx_cfg1);

}

void hsv_wave(ws2812_rgb_struct * rgb_struct, fx_config * p_fx){

	struct_HSV _hsv = { fx_cfg1->hue_offset, 1000, 50, 0 };
	UINT32_RGB _rgb = { { 0, 0, 0, 0 } };
	uint8_t i_sin = 0;

	for (uint16_t i = 0; i < p_fx->number_pixels; ++i) {
		_hsv.hue++;
		_hsv.val = (((uint16_t) sine_wave[i_sin] / 16));
		_rgb = hsv2rgb(&_hsv);
		set_pixel_GRB(rgb_struct, &_rgb, map_to_pixel(i));
		i_sin < 255 ? i_sin++ : 0;
	}

	shift_hue(fx_cfg1);

}




void rgb_wave(ws2812_rgb_struct *rgb_struct, fx_config * p_fx) {
    static float time;
    float xx;
    UINT32_RGB _rgb = { { 0, 0, 0, 0 } };


    if(time > 2*M_PI) {
        time = 0.0;
    }

    static uint16_t index = 0;
    for(uint8_t y = 0; y < p_fx->number_rows; y++) {
        for(uint8_t x = 0; x < p_fx->number_columns; x++) {
            xx = mapf(x, 0, p_fx->number_columns-1, 0, 2*M_PI);
            _rgb.xRGB.red = 16 + 100 * (bound(sinf(xx + time + 2*M_PI/3), 0.5, -0.5) + 0.5);
            _rgb.xRGB.green = 16 + 100 * (bound(sinf(xx + time - 2*M_PI/3), 0.5, -0.5) + 0.5);
            _rgb.xRGB.blue = 16 + 100 * (bound(sinf(xx + time         ), 0.5, -0.5) + 0.5);
            set_pixel_GRB(rgb_struct, &_rgb, map_to_pixel(index));
            index++;
        }
    }
    time += 0.1;
}

void plasma_fill(ws2812_rgb_struct *rgb_struct, fx_config * p_fx) {
    static float time;
    float xx, yy;
    float v;
    float delta = 0.025;

    time += 0.025;
    if(time > 12*M_PI) {
        delta *= -1;
    }

    for(uint8_t y = 0; y < p_fx->number_rows; y++) {
        yy = mapf(y, 0, ((p_fx->number_rows)-1), 0, 2*M_PI);
        for(uint8_t x = 0; x < p_fx->number_columns; x++) {
            xx = mapf(x, 0, ((p_fx->number_columns)-1), 0, 2*M_PI);

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








void fire_fill(ws2812_rgb_struct *_ws_struct, fx_config * p_fx) {

}

}

void setColorBrightness(const UINT32_RGB * in, UINT32_RGB * out, float brightness)
{
    out->xRGB.red   = brightness * in->xRGB.red;
    out->xRGB.green = brightness * in->xRGB.green;
    out->xRGB.blue  = brightness * in->xRGB.blue;
}

void RunningLights(UINT32_RGB * c, uint32_t delay_ms, uint32_t time_s , fx_config * p_fx)
{
	static UINT32_RGB new_color;
    int num_of_frames = time_s * 1000 / delay_ms;
    for (int j = 0; j < num_of_frames; j++)
    {
        for (uint16_t i = 0; i < p_fx->number_pixels; i++)
        {
        //    setColorBrightness(c,&new_color,(sin(i + j) * 127 + 128) / 255.0);
       //     setPixel_GRB(&new_color, i);
        }

    }
}


void perlin(ws2812_rgb_struct *_ws_struct, fx_config * p_fx)
{

}








