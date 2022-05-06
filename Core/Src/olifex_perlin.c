/*
 * olifex_perlin.c
 *
 *  Created on: May 4, 2022
 *      Author: omwfo
 */


#include "fastnoise.h"
#include "olifex_perlin.h"
#include "stdlib.h"

void fill_rnd_vectors(fx_vec_theta *p_vec,uint16_t num_vec)
{

	for(uint16_t i = 0; i<=(num_vec);i++)
	{
		p_vec->mag = (q15_t)RAND_VECS_2D[i];
		i++;
		p_vec->theta = (q15_t)RAND_VECS_2D[i];
		i++;
		p_vec++;
	}
}



void theta_to_coord(fx_vec_theta *  fx_t,fx_vec_coord * fx_c)
{

	fx_c->x = acos(fx_t->theta);
	fx_c->y = asin(fx_t->theta);

}


q15_t dot_product_2d(fx_vec_theta * v1,fx_vec_coord * v2)
{

	fx_vec_coord v_temp = {0,0};
	theta_to_coord(v1,&v_temp);
	return (((v_temp.x)*(v2->x))+((v_temp.y)*(v2->y)));

}








q15_t lerp(fx_vec_coord * v1,fx_vec_coord * v2,uint16_t cell_size)
{



}
void init_perlin(ws2812_rgb_struct *_ws_struct, fx_config * p_fx)
{

	uint16_t n_vec;



	uint16_t leds_per_cell = p_fx->grad_cells.cell_size_x * p_fx->grad_cells.cell_size__y;
//	p_fx->gradient_cells.cell_n =
	if(((p_fx->n_pixels) % (leds_per_cell))==0)
	{
//		n_vec  =((p_fx->grad_cells.cell_x)*(p_fx->grad_cells.cell_y));
	}
	else
	{
//		n_vec  =(((p_fx->grad_cells.cell_x)+1)*((p_fx->grad_cells.cell_y)+1));
	}
	//todo:calculate how many rows or columns are required for unit vectors

	p_fx->grad_cells.grad_vectors  = malloc(n_vec*(sizeof(fx_vec_theta)));


	fill_rnd_vectors(p_fx->grad_cells.grad_vectors,n_vec);

	for(uint16_t i = 0;i <= (_ws_struct->length);++i)
	{


	}
}

void perlin(ws2812_rgb_struct *_ws_struct, fx_config * p_fx)
{
	uint16_t a,b,c,d;
	uint16_t column_offset = p_fx->row_len/p_fx->grad_cells.cells_y;
	uint16_t row_offset = p_fx->col_len;
	fx_vec_theta * cursor[4];

		for(uint16_t i = 0 ; i < p_fx->n_pixels;i+=p_fx->grad_cells.cells_x){
			// iterate rows
			cursor[0] = &p_fx->grad_cells.grad_vectors[row_offset];


			// iterate columns

			for(uint16_t i  = 0; i<=p_fx->n_pixels;i++)
				{

				//increment column offset
				}
			//iterate row offset
		}





}












