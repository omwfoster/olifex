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

void fill_rnd_vectors(fx_polar_coord *p_vec, uint16_t num_vec) {

	for (uint16_t i = 0; i <= (num_vec); i++) {
		p_vec->mag = 1;
		p_vec->theta = (q15_t) wyhash16();
		i++;
		p_vec++;
	}
}

void vector_rotate(fx_polar_coord *p_vec, uint16_t num_vec, q15_t step) {
	float32_t fpc = (float32_t) (*p_vec).theta;
	q15_t step1 = 0x4000;
	float32_t q_theta = fpc + (float32_t) step;

	for (uint16_t i = 0; i < num_vec; i++) {
		q_theta = fpc;
		q_theta += step1;
		(*p_vec).theta = (q15_t) q_theta;
		fpc++;
	}
}

fx_vec_coord polar_to_vector(fx_polar_coord *v1) {
	fx_vec_coord v2;
	v2.x = acos(v1->theta * TWOPI);
	v2.y = asin(v1->theta * TWOPI);
	return v2;
}

q15_t lin_inter(q15_t x, q15_t y, q15_t s) {
	return x + s * (y - x);
}

q15_t smooth_inter(q15_t x, q15_t y, q15_t s) {
	return lin_inter(x, y, s * s * (3 - 2 * s));
}

q15_t dot_product(fx_vec_coord *a, fx_vec_coord *b) {
	return ((a->x * b->x) + (a->y * b->y));
}

q15_t lerp(fx_vec_coord *vector, rnd_v *vector_cell, q15_t s) {
	q15_t a = dot_product(vector, &vector_cell->v_a);
	q15_t b = dot_product(vector, &vector_cell->v_b);
	q15_t c = dot_product(vector, &vector_cell->v_c);
	q15_t d = dot_product(vector, &vector_cell->v_d);
	q15_t x0 = smooth_inter(a, b, s);
	q15_t x1 = smooth_inter(c, d, s);
	q15_t y0 = smooth_inter(x0, x1, s);

	return y0;
}

void get_corner_vectors(uint16_t index, fx_config *p_fx, rnd_v *p_vec) {

	uint16_t a, b, c, d = 0;
	a = index;
	b = (index + (*p_fx).grad_cells.cell_size_x);
	c = (index + (2 * (*p_fx).grad_cells.cells_x));
	d = (c + (*p_fx).grad_cells.cell_size_x);
	(*p_vec).v_a = polar_to_vector(&p_fx->grad_cells.grad_vectors[a]);
	(*p_vec).v_b = polar_to_vector(&p_fx->grad_cells.grad_vectors[b]);
	(*p_vec).v_c = polar_to_vector(&p_fx->grad_cells.grad_vectors[c]);
	(*p_vec).v_d = polar_to_vector(&p_fx->grad_cells.grad_vectors[d]);
}

uint16_t find_last(uint16_t index, uint16_t row_offset, uint16_t cell_size,
		uint16_t n_pixels) {
	index += cell_size;
	uint16_t next;

	for (int i = 0; i < cell_size; ++i) {
		next = index + row_offset;
		index = next < n_pixels ? next : index;

	}
return index;
}

uint16_t find_eol(uint16_t index, uint16_t row_len) {
	return (index > 0 ? (uint16_t) ceil(index / row_len) : row_len);
}

uint16_t draw_cell_ws(uint16_t index, ws2812_rgb_struct *ws, fx_config *p_fx) {

	uint16_t eol = find_eol(index, (*ws).length);
	uint16_t row_offset = ((eol - index) + 1);
	rnd_v v_corners = { { 0 } };
	UCOL Color;
	int k = 0;

	get_corner_vectors(index, p_fx, &v_corners);

	//find eol

	for (int i = 0; i < (*p_fx).grad_cells.cells_y; i++) {

		for (int j = 0; j < (*p_fx).grad_cells.cells_y; j++)

		{
			if (index < eol) {

				q15_t intensity = lerp(&three_square[k], &v_corners, 0.15);

				Color.xRGB.red = 0;
				Color.xRGB.green = 0;
				Color.xRGB.blue = (uint8_t) ((float32_t) intensity * 255);
				set_pixel_GRB(ws, &Color, index);

				k++;
			}
			j++;
			index += row_offset;
		}

		i++;


	}

}

void perlin(ws2812_rgb_struct *ws, fx_config *p_fx) {

	q15_t step = 1307;

	rnd_v v_t = { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } };
	uint16_t index_low = 0;
	uint16_t eol = find_eol(index_low, (*ws).n_row);

	while (index_low < (*ws).length) { //do until start point is larger than n_pixels
		//calculate row ceiling;

		eol = find_eol((*p_fx).grad_cells.cells_x, (*ws).n_row);

		while (index_low < eol) {

			get_corner_vectors(index_low, p_fx, &v_t);
			draw_cell_ws(index_low, ws, p_fx);

		}

		index_low += (2 * (*ws).n_col);

	}

	vector_rotate(p_fx->grad_cells.grad_vectors, (*p_fx).grad_cells.n_vectors,
			step);

}

