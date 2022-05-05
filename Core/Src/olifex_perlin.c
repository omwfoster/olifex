/*
 * olifex_perlin.c
 *
 *  Created on: May 4, 2022
 *      Author: omwfo
 */


#include "fastnoise.h"
#include "olifex_perlin.h"
#include "stdlib.h"

void fill_rnd_vectors(fx_vec *p_vec,uint16_t num_vec)
{

	for(uint16_t i = 0; i<=(num_vec);i++)
	{
		p_vec->x = (q15_t)RAND_VECS_2D[i];
		i++;
		p_vec->y = (q15_t)RAND_VECS_2D[i];
		i++;
		p_vec++;
	}
}


q15_t dot_product_2d(fx_vec v1,fx_vec v2)
{
		q15_t dot_p = 0.0;
		dot_p += v1.x*v2.x;
		dot_p += v1.y*v2.y;

    return dot_p;

}


uint16_t top_left(uint16_t n,uint16_t divisor)
{
	if(n==0){return 0;}
	if(n%divisor==0){return(n/divisor);}
	else{return (n/divisor);} /// incorrect

}
uint16_t bottom(uint16_t n,fx_config * p_fx)
{
    return(n+((p_fx->grad_cells.cell_y)*(p_fx->row_len)));
}

uint16_t top_right(uint16_t n_left,fx_config * p_fx)
{
	return(n_left+(p_fx->grad_cells.cell_x));

}





q15_t lerp(fx_vec * v1,fx_vec * v2,uint16_t cell_size)
{



}
void init_perlin(ws2812_rgb_struct *_ws_struct, fx_config * p_fx)
{

	uint16_t n_vec;



	uint16_t leds_per_cell = p_fx->grad_cells.cell_x * p_fx->grad_cells.cell_y;
//	p_fx->gradient_cells.cell_n =
	if(((p_fx->n_pixels) % (leds_per_cell))==0)
	{
		n_vec  =((p_fx->grad_cells.cell_x)*(p_fx->grad_cells.cell_y));
	}
	else
	{
		n_vec  =(((p_fx->grad_cells.cell_x)+1)*((p_fx->grad_cells.cell_y)+1));
	}
	//todo:calculate how many rows or columns are required for unit vectors

	p_fx->grad_cells.grad_vectors  = malloc(n_vec*(sizeof(fx_vec)));


	fill_rnd_vectors(p_fx->grad_cells.grad_vectors,n_vec);

	for(uint16_t i = 0;i <= (_ws_struct->length);++i)
	{


	}
}

void perlin(ws2812_rgb_struct *_ws_struct, fx_config * p_fx)
{
	for(uint16_t i  = 0; i<=p_fx->n_pixels;i++)
	{
		top_left(i,p_fx->col_len);
	}

}



void fill_perlin_cell(ws2812_rgb_struct *_ws_struct, fx_config * p_fx)
{

	for(uint16_t i = 0;i<p_fx->grad_cells.cell_n;i++)
		{

		}


}










