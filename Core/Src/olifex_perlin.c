/*
 * olifex_perlin.c
 *
 *  Created on: May 4, 2022
 *      Author: omwfo
 */


#include "fastnoise.h"
#include "olifex_perlin.h"
#include "olifex_pixel.h"
#include "stdlib.h"

uint16_t wyhash16_x = 4778;

uint32_t hash16(uint32_t input, uint32_t key) {
  uint32_t hash = input * key;
  return ((hash >> 16) ^ hash) & 0xFFFF;
}

uint16_t wyhash16() {
  wyhash16_x += 0xfc15;
  return hash16(wyhash16_x, 0x2ab);
}

void fill_rnd_vectors(fx_polar_coord *p_vec,uint16_t num_vec)
{

	for(uint16_t i = 0; i<=(num_vec);i++)
	{
		p_vec->mag = 1;
		p_vec->theta = (q15_t)wyhash16();
		i++;
		p_vec++;
	}
}

void vector_rotate(fx_polar_coord * p_vec, uint16_t num_vec, q15_t step)
{
	float32_t fpc = (float32_t)(*p_vec).theta;
	q15_t step1 =  0x4000;
	float32_t q_theta = fpc + (float32_t)step;

	for(uint16_t i = 0; i < num_vec ; i++)
	{
		q_theta = fpc;
		q_theta += step1;
		(*p_vec).theta = (q15_t)q_theta;
		fpc++;
	}
}

fx_vec_coord polar_to_vector(fx_polar_coord * v1)
{
	    fx_vec_coord v2;
		v2.x = acos(v1->theta*TWOPI);
		v2.y = asin(v1->theta*TWOPI);
		return v2;
}


q15_t lin_inter(q15_t x, q15_t y, q15_t s)
{
    return x + s * (y-x);
}

q15_t smooth_inter(q15_t x, q15_t y, q15_t s)
{
    return lin_inter(x, y, s * s * (3-2*s));
}

q15_t dot_product(fx_vec_coord  * a, fx_vec_coord *  b)
{
	return ((a->x * b->x) + (a->y * b->y));
}

q15_t lerp(fx_vec_coord * vector  ,rnd_v * vector_cell, q15_t s)
{


	q15_t a = dot_product(vector,&vector_cell->v_a);
	q15_t b = dot_product(vector,&vector_cell->v_b);
	q15_t c = dot_product(vector,&vector_cell->v_c);
	q15_t d = dot_product(vector,&vector_cell->v_d);
	q15_t x0 = smooth_inter(a,b,s);
	q15_t x1 = smooth_inter(c,d,s);
	q15_t y0 = smooth_inter(x0,x1,s);

return y0;
}



void calc_cell_fx(fx_config * p_fx,cell * p_cell,rnd_v * p_vec)
{



	 (*p_cell).b = ((*p_cell).a + (*p_fx).grad_cells.cell_size_x);
	 (*p_cell).c = (((*p_cell).a + (*p_fx).grad_cells.cell_size_y - 1) * (*p_fx).grad_cells.row_offset);
	 (*p_cell).d = ((*p_cell).c + ((*p_fx).grad_cells.cell_size_x));
	 (*p_vec).v_a = polar_to_vector(&(*p_fx).grad_cells.grad_vectors[p_cell->a]);
	 (*p_vec).v_b = polar_to_vector(&p_fx->grad_cells.grad_vectors[p_cell->b]);
	 (*p_vec).v_c = polar_to_vector(&p_fx->grad_cells.grad_vectors[p_cell->c]);
	 (*p_vec).v_d = polar_to_vector(&p_fx->grad_cells.grad_vectors[p_cell->d]);
}

void calc_cell_ws(ws2812_rgb_struct *p_ws,cell * p_cell,rnd_v * corner_vectors)

{  uint16_t  next, i,j,n_x,n_y,k = 0;


    n_x = ((p_cell->b) - (p_cell->a));
	n_y = ((p_cell->c - p_cell->a)/(p_cell->b - p_cell->a));
	p_ws->cursor = &p_ws->ptr_start[p_cell->a];
	next = p_ws->n_row - n_y;
	uint8_t output_level = 0;
	//uint16_t start = p_cell->a;
	uint16_t  ws_index;
	q15_t s;


	for(i=0;i<n_y;i++)
     {
		s = p_cell->a / n_y;

    	for(j=0;j<n_x;j++)
    	{
    		output_level = (0xFF * (lerp(&three_square[k],corner_vectors,s)));
    	    blend((UCOL*)&RGB_RED,(UCOL*)&RGB_GREEN, (*p_ws).cursor,output_level);

    		p_ws->cursor++;
    		ws_index++;
    		k++;

    	}
    	p_ws->cursor += next;

     }

}



void perlin(ws2812_rgb_struct *ws, fx_config * p_fx){

	q15_t step = 1307;
	cell c1 = {0,0,0,0};
	rnd_v v_t = {{0,0},{0,0},{0,0},{0,0}};
	for(uint16_t j = 0;j<p_fx->grad_cells.cells_y;++j)
	{
		for(uint16_t i = 0;i<p_fx->grad_cells.cells_x;++i)
			// iterate rows
					{
				    calc_cell_fx(p_fx,&c1,&v_t);
				    calc_cell_ws(ws,&c1,&v_t);

					c1.a = c1.b + 1 ;

					//get_corners
					}

			c1.a += ((p_fx->grad_cells.row_offset) * (p_fx->grad_cells.cell_size_y - 1)) ;

			}

	vector_rotate(p_fx->grad_cells.grad_vectors, (*p_fx).grad_cells.n_vectors , step);



}




















