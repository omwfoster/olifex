/*
 * olifex_perlin.c
 *
 *  Created on: May 4, 2022
 *      Author: omwfo
 */


#include "fastnoise.h"
#include "olifex_perlin.h"
#include "stdlib.h"

void fill_rnd_vectors(fx_polar_coord *p_vec,uint16_t num_vec)
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


fx_vec_coord polar_to_vector(fx_polar_coord * v1)
{
	    fx_vec_coord v2;
		v2.x = acos(v1->theta*TWOPI)*v1->mag;
		v2.y = asin(v1->theta*TWOPI)*v1->mag;
		return v2;
}


float32_t lin_inter(float32_t x, float32_t y, float32_t s)
{
    return x + s * (y-x);
}

float32_t smooth_inter(float32_t x, float32_t y, float32_t s)
{
    return lin_inter(x, y, s * s * (3-2*s));
}

q15_t dot_product(fx_vec_coord a, fx_vec_coord b)
{
	return ((a.x * b.x) + (a.y * b.y));
}

q15_t lerp(fx_vec_coord vector  ,rnd_v * vector_cell)
{
	q15_t a = dot_product(vector,vector_cell->v_a);
	q15_t b = dot_product(vector,vector_cell->v_b);
	q15_t c = dot_product(vector,vector_cell->v_c);
	q15_t d = dot_product(vector,vector_cell->v_d);


}







void calc_cell_fx(fx_config * p_fx,cell * p_cell,rnd_v * corner_vectors)
{
	 p_cell->b = ((p_cell->a) + (p_fx->grad_cells.cell_size_x));
	 p_cell->c = ((p_cell->a) + (p_fx->grad_cells.cell_size_y - 1) * (p_fx->grad_cells.row_offset));
	 p_cell->d = ((p_cell->c) + (p_fx->grad_cells.cell_size_x));
	 corner_vectors->v_a = polar_to_vector(&p_fx->grad_cells.grad_vectors[p_cell->a]);
	 corner_vectors->v_b = polar_to_vector(&p_fx->grad_cells.grad_vectors[p_cell->b]);
	 corner_vectors->v_c = polar_to_vector(&p_fx->grad_cells.grad_vectors[p_cell->c]);
	 corner_vectors->v_d = polar_to_vector(&p_fx->grad_cells.grad_vectors[p_cell->d]);
}

void calc_cell_ws(ws2812_rgb_struct *p_ws,cell * p_cell,rnd_v * corner_vectors)

{  uint16_t start , i,j,x_len,y_len = 0;


    x_len = ((p_cell->b) - (p_cell->a));
	y_len = ((p_cell->c - p_cell->a)/(p_cell->b - p_cell->a));
<<<<<<< HEAD
	p_ws->cursor = &p_ws->ptr_start[p_cell->a];
	next = p_ws->n_row - y_len;
	start = p_cell->a;
	uint16_t  ws_index;
	q15_t s;
=======
	* p_ws->cursor = p_ws->ptr_start[p_cell->a];
>>>>>>> parent of 816edf7... cleanup

	for(i=0;i<y_len;i++)
     {
    	for(j=0;j<x_len;j++)
    	{



    		* p_ws->cursor++;
    	}

    	p_cell->a = start + p_ws->n_row;


     }



}




void perlin(ws2812_rgb_struct *ws, fx_config * p_fx){

	cell c1 = {0,0,0,0};
	rnd_v v_t = {{0,0},{0,0},{0,0},{0,0}};

		while(c1.c < (p_fx->n_pixels)){
			// iterate rows

			while(c1.b < (ws->n_row))
					{
				    calc_cell_fx(p_fx,&c1,&v_t);

					c1.a = c1.b + 1 ;

					//get_corners
					}
<<<<<<< HEAD
			calc_cell_ws(ws,&c1,&v_t);
			c1.a += ((p_fx->grad_cells.row_offset) * (p_fx->grad_cells.cell_size_y - 1)) ;
=======

			c1.a = ceil((c1.d)/(p_fx->row_len))*(p_fx->row_len);
>>>>>>> parent of 816edf7... cleanup

			}





}


















