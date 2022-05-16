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



void theta_to_coord(fx_polar_coord * v1,fx_vec_coord * v2)
{
		v2->x = acos(v1->theta*TWOPI)*v1->mag;
		v2->y = asin(v1->theta*TWOPI)*v1->mag;
}


void corner_vectors(fx_polar_coord * p1,fx_vec_coord * v_t ,cell * c1)
{


		theta_to_coord(&p1[c1->a],&v_t[0]);
		theta_to_coord(&p1[c1->b],&v_t[1]);
		theta_to_coord(&p1[c1->c],&v_t[2]);
		theta_to_coord(&p1[c1->d],&v_t[3]);




}


void lerp(cell * p_cell, fx_polar_coord * p_gv)
{



}


void dot_product(fx_vec_coord cell_vector  ,fx_vec_coord * v_c)
{



}









void init_cell(uint16_t x_n, uint16_t y_n,fx_cells * p_fx)
{

}

void calc_cell_fx(fx_config * p_fx,cell * p_cell)
{
	 p_cell->b = ((p_cell->a) + (p_fx->grad_cells.cell_size_x));
	 p_cell->c = ((p_cell->a) + (p_fx->grad_cells.cell_size_y - 1));
	 p_cell->d = ((p_cell->c) + (p_fx->grad_cells.cell_size_x));
}

void calc_cell_ws(ws2812_rgb_struct *p_ws,cell * p_cell,fx_vec_coord  * v_t)

{  uint16_t start , i,j,x_len,y_len = 0;


    x_len = (p_cell->b - p_cell->a);
	y_len = ((p_cell->c - p_cell->a)/(p_cell->b - p_cell->a));
     for(i=0;i<y_len;i++)
     {
    	for(j=0;j<x_len;j++)
    	{

   // 		p_ws->ptr_start[p_cell->a] = (UCOL)(dot_product(three_square[cursor],&v_t[0])).xUINT;

    	}

    	p_cell->a = start + p_ws->n_row;


     }



}




void perlin(ws2812_rgb_struct *ws, fx_config * p_fx){

	cell c1 = {0,0,0,0};
	fx_vec_coord v_t[4] = {{0,0},{0,0},{0,0},{0,0}};





		while(c1.c < (p_fx->n_pixels)){
			// iterate rows



			while(c1.b < (ws->n_row))
					{

					calc_cell_fx(p_fx,&c1);
					// iterate full columns.
					corner_vectors(&p_fx->grad_cells.grad_vectors[0],&v_t[0],&c1);

					c1.a = c1.b + 1 ;




					//get_corners


					}


			c1.a = ceil((c1.d)/(p_fx->row_len))*(p_fx->row_len);





			}








				// iterate full columns.

				//get_corners



			//iterate last column



			//bottom right


}


















