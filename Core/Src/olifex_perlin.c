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



fx_vec_coord theta_to_coord(fx_vec_theta *  fx_t)
{

	//fx_c->x = acos(fx_t->theta);
	//fx_c->y = asin(fx_t->theta);
//	return(fx_vec_coord){acos(fx_t->theta),asin(fx_t->theta}
}


q15_t dot_product_2d(fx_vec_theta * v1,fx_vec_coord * v2)
{

	fx_vec_coord v_temp = {0,0};
//	theta_to_coord(v1,&v_temp);
//	return (((v_temp.x)*(v2->x))+((v_temp.y)*(v2->y)));

}








q15_t lerp(fx_vec_coord * v1,fx_vec_coord * v2,uint16_t cell_size)
{



}
void init_perlin(ws2812_rgb_struct *_ws_struct, fx_config * p_fx)
{

	float32_t x = ceil((float32_t)p_fx->row_len/(float32_t)p_fx->grad_cells.cell_size_x);

	x*= p_fx->grad_cells.cell_size_x;
	p_fx->grad_cells.cells_x = (uint16_t)x;

	float32_t y = ceil((float32_t)p_fx->n_pixels)*((float32_t)p_fx->col_len*(float32_t)(p_fx->grad_cells.cell_size_y));

	y*= p_fx->grad_cells.cell_size_y;
	p_fx->grad_cells.cells_y = (uint16_t)y;

	uint32_t n_elements = (((p_fx->grad_cells.cells_x)*(p_fx->grad_cells.cell_size_x))
			*((p_fx->grad_cells.cells_y)*(p_fx->grad_cells.cell_size_y)));



	p_fx->grad_cells.grad_vectors  = malloc(n_elements*(sizeof(fx_vec_theta)));

	p_fx->grad_cells.col_offset = p_fx->grad_cells.cell_size_y;

	p_fx->grad_cells.row_offset = (((p_fx->grad_cells.cells_x)*(p_fx->grad_cells.cell_size_x))
			*(p_fx->grad_cells.cell_size_y));



	fill_rnd_vectors(p_fx->grad_cells.grad_vectors,n_elements);

	for(uint16_t i = 0;i <= (_ws_struct->length);++i)
	{


	}
}

void perlin(ws2812_rgb_struct *_ws_struct, fx_config * p_fx)
{


	fx_cells * fx = &p_fx->grad_cells;
	fx_vec_theta * cursor;
	fx_vec_theta * cell[4];


	for(uint16_t i = 0 ; i < fx->cells_x;i++){

			for(uint16_t j = 0 ; j < fx->cells_y;j++){
				// iterate rows
				cursor  = &fx->grad_vectors[((fx->row_offset)*(j))];
				cell[0] = cursor;
				cell[1] = cursor + fx->col_offset;
				cell[2] = cursor + fx->row_offset;
				cell[0] = cursor + fx->row_offset +fx->col_offset;
			}



	}





}












