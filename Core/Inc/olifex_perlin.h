/*
 * olifex_perlin.h
 *
 *  Created on: May 4, 2022
 *      Author: olifoster
 */

#ifndef INC_OLIFEX_PERLIN_H_
#define INC_OLIFEX_PERLIN_H_

#include "math.h"
#include <arm_math.h>
#include <olifex_pixel.h>
#include <olifex_fx.h>
#include "tables.h"


static const fx_vec_coord three_square[]= {{-1,-1},{1,1},{1,0},{-1,-1},{0,1},{1,-1},{0,-1},{0,0}};
void perlin(ws2812_rgb_struct *_ws_struct, fx_config * p_fx);
void fill_rnd_vectors(fx_polar_coord *p_vec,uint16_t num_vec);
void vector_rotate(fx_polar_coord * p_vec, uint16_t num_vec, q15_t step);

#define TWOPI 6.28

typedef struct cell{
	uint16_t a;
	uint16_t b;
	uint16_t c;
	uint16_t d;
}cell;

typedef struct rnd_v{
	fx_vec_coord v_a;
	fx_vec_coord v_b;
	fx_vec_coord v_c;
	fx_vec_coord v_d;
}rnd_v;


#endif /* INC_OLIFEX_PERLIN_H_ */





