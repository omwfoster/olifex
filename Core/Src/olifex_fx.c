/*

 */
#include "olifex_fx.h"
#include "olifex_perlin.h"

#include "stdlib.h"
#include "tables.h"

#define PIXEL_ROWS					5
#define PIXEL_COLUMNS				8


bool fx_init = false;


float mapf(float value, float c_min, float c_max, float t_min, float t_max) {
    return (value - c_min) / (c_max - c_min) * (t_max - t_min) + t_min;
}

float bound(float value, float max, float min) {
    return fmaxf(fminf(value, max), min);
}







void init_olifex_fx(fx_config *p_fx, uint16_t col_len,uint16_t row_len,uint16_t cell_size) {


	fx_init = true;

	(*p_fx).n_pixels = PIXEL_ROWS * PIXEL_COLUMNS;
	(*p_fx).grad_cells.cell_size_x = cell_size;
	(*p_fx).grad_cells.cell_size_y = cell_size;
	(*p_fx).grad_cells.cells_x = ceil(((float32_t)row_len/(float32_t)cell_size));
	(*p_fx).grad_cells.cells_y = ceil(((float32_t)col_len/(float32_t)cell_size));
	(*p_fx).grad_cells.row_offset = (uint16_t)(*p_fx).grad_cells.cells_x * cell_size;
	(*p_fx).grad_cells.col_offset = cell_size;
	(*p_fx).grad_cells.n_vectors = ((((*p_fx).grad_cells.cells_x)+1)*(((*p_fx).grad_cells.cells_y)+1));
	(*p_fx).grad_cells.grad_vectors = malloc(((*p_fx).grad_cells.n_vectors) * sizeof(fx_polar_coord));


	(*p_fx).row_len = PIXEL_ROWS;
	(*p_fx).col_len = PIXEL_COLUMNS;
	(*p_fx).map_xy  = malloc(p_fx->n_pixels * sizeof(uint16_t));
	(*p_fx).direction     = true;
	(*p_fx).pixel_array   = malloc((p_fx->n_pixels)*(sizeof(UCOL)));
	(*p_fx).val_offset    = 32;


	fill_rnd_vectors(p_fx->grad_cells.grad_vectors,(*p_fx).grad_cells.n_vectors);
	fill_pixel_map(p_fx);

}



uint8_t fill_pixel_map(fx_config *p_fx)
{

	uint16_t i = 0;
	if((!(p_fx->row_len)==0))
	{
		for(i = 0;i<p_fx->n_pixels;i++)
 	    p_fx->map_xy[i]=i;
		return 0 ;
	}

    // Traverse through all rows
    for (i = 1; i <= (p_fx->row_len); i++) {

        // If current row is even, print from
        // left to right
        if (i % 2 == 0) {
            for (uint16_t j = 0; j < (p_fx->col_len); j++)
            	p_fx->map_xy[i]=i;

        // If current row is odd, print from
        // right to left
        } else {
            for (uint16_t j = (((p_fx->col_len) * i)-1) ; j >=  (((p_fx->col_len) * (i-1)-1)); j--)

            p_fx->map_xy[i]=j;
        }
    }
	return 1;
}


uint16_t map_to_pixel(uint16_t i,fx_config *p_fx)
{
	return i<(p_fx->n_pixels)?(p_fx->map_xy)[i]:0;
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

	UCOL rgb = { { 0, 0, 0, 0 } };

	uint32_t temp;
	for (uint8_t i = 0; i < p_fx->n_pixels; ++i) {
		p_fx->pos_offset < 0xFF ? p_fx->pos_offset++ : 0;
		temp = (color_wheel(p_fx->pos_offset, p_fx->val_offset));
		rgb.xUINT = temp;
		set_pixel_GRB(rgb_struct, &rgb, i);
	}

	shift_pos(p_fx);

}

void hsv_scroll(ws2812_rgb_struct *rgb_struct, fx_config * p_fx) {

	XHSV _hsv = { p_fx->hue_offset, 1000, p_fx->val_offset, 0 };
	UCOL _rgb = { { 0, 0, 0, 0 } };

	for (uint16_t i = 0; i < p_fx->n_pixels; ++i) {
		_hsv.hue++;
		_rgb = hsv2rgb(&_hsv);
		set_pixel_GRB(rgb_struct, &_rgb, map_to_pixel(i,p_fx));
	}

	shift_hue(p_fx);

}

void hsv_wave(ws2812_rgb_struct * rgb_struct, fx_config * p_fx){

	XHSV _hsv = { p_fx->hue_offset, 1000, p_fx->val_offset, 0 };
	UCOL _rgb = { { 0, 0, 0, 0 } };
	uint8_t i_sin = 0;

	for (uint16_t i = 0; i < p_fx->n_pixels; ++i) {
		_hsv.hue++;
		_hsv.val = (((uint16_t) sine_wave[i_sin] / 16));
		_rgb = hsv2rgb(&_hsv);
		set_pixel_GRB(rgb_struct, &_rgb, map_to_pixel(i,p_fx));
		i_sin < 255 ? i_sin++ : 0;
	}

	shift_hue(p_fx);

}




void rgb_wave(ws2812_rgb_struct *rgb_struct, fx_config * p_fx) {
    static float time;
    float xx;
    UCOL _rgb = { { 0, 0, 0, 0 } };


    if(time > 2*M_PI) {
        time = 0.0;
    }

    static uint16_t index = 0;
    for(uint8_t y = 0; y < p_fx->row_len; y++) {
        for(uint8_t x = 0; x < p_fx->col_len; x++) {
            xx = mapf(x, 0, p_fx->col_len-1, 0, 2*M_PI);
            _rgb.xRGB.red = 16 + 100 * (bound(sinf(xx + time + 2*M_PI/3), 0.5, -0.5) + 0.5);
            _rgb.xRGB.green = 16 + 100 * (bound(sinf(xx + time - 2*M_PI/3), 0.5, -0.5) + 0.5);
            _rgb.xRGB.blue = 16 + 100 * (bound(sinf(xx + time         ), 0.5, -0.5) + 0.5);
            set_pixel_GRB(rgb_struct, &_rgb, map_to_pixel(index,p_fx));
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

    for(uint8_t y = 0; y < p_fx->row_len; y++) {
        yy = mapf(y, 0, ((p_fx->row_len)-1), 0, 2*M_PI);
        for(uint8_t x = 0; x < p_fx->col_len; x++) {
            xx = mapf(x, 0, ((p_fx->col_len)-1), 0, 2*M_PI);

            v = sinf(xx + time);
            v += sinf((yy + time) / 2.0);
            v += sinf((xx + yy + time) / 2.0);
            float cx = xx + .5 * sinf(time/5.0);
            float cy = yy + .5 * cosf(time/3.0);
            v += sinf(sqrtf((cx*cx+cy*cy)+1)+time);
            v /= 2.0;

        }
    }
}


void fire_fill(ws2812_rgb_struct *_ws_struct, fx_config * p_fx) {

}


void setColorBrightness(const UCOL * in, UCOL * out, float brightness)
{
    out->xRGB.red   = brightness * in->xRGB.red;
    out->xRGB.green = brightness * in->xRGB.green;
    out->xRGB.blue  = brightness * in->xRGB.blue;
}

void RunningLights(UCOL * c, uint32_t delay_ms, uint32_t time_s , fx_config * p_fx)
{

    int num_of_frames = time_s * 1000 / delay_ms;
    for (int j = 0; j < num_of_frames; j++)
    {
        for (uint16_t i = 0; i < p_fx->n_pixels; i++)
        {

        }

    }
}


void test_gradient( ws2812_rgb_struct *ws, fx_config *p_fx)
 {

	UCOL Color;

	for (uint16_t y = 0; y < (*p_fx).n_pixels; y++) {

				Color.xRGB.red = y ;
				Color.xRGB.green = 0 ;
				Color.xRGB.blue = 255 - y ;
				set_pixel_GRB(ws, &Color, y);

	}

}














