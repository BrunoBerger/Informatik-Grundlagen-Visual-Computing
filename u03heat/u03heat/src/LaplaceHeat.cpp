#include "LaplaceHeat.h"
#include <algorithm>
#include <iostream>
#include <map>

/*
 *
 */
LaplaceHeat::LaplaceHeat(int xsize, int ysize, double heat_min, double heat_max, int steps, int niter)
		: XSIZE(xsize), YSIZE(ysize), HEAT_MIN(heat_min), HEAT_MAX(heat_max), STEPS(steps), NITER(niter) {

	YSIZE_PLUS_2 = YSIZE + 2;
	// Additional 2 boundary cells to simplify calculation.
	h     = new double[(XSIZE + 2) * (YSIZE_PLUS_2)]; // Finite difference array.
	h_new = new double[(XSIZE + 2) * (YSIZE_PLUS_2)]; // Finite difference array (new values).

}

/*
 *
 */
LaplaceHeat::~LaplaceHeat() {

	delete[] h;
	delete[] h_new;
}

/*
 * Finite difference method (Laplace).
 */
void LaplaceHeat::update() {
	// Calculate new value based upon neighbor values.

	for (int i = 1; i <= XSIZE; ++i){	
		for (int j = 1; j <= YSIZE; ++j){
			int IxYp2pJ = i * YSIZE_PLUS_2 + j;

			h_new[IxYp2pJ] =  0.25 * (
				h[(i - 1) * YSIZE_PLUS_2 + j] +
				h[IxYp2pJ - 1] + 
				h[IxYp2pJ + 1] + 
				h[(i + 1) * YSIZE_PLUS_2 + j] 
			);

			// int index_1 = (i - 1) * YSIZE_PLUS_2 + j;
			// int index_2 = IxYp2pJ - 1;
			// int index_3 = IxYp2pJ + 1;
			// int index_4 = (i + 1) * YSIZE_PLUS_2 + j;
			// double newVal = 0.25 * (h[index_1] + h[index_2] + h[index_3] + h[index_4] );
			// h_new[IxYp2pJ] = newVal;
			
			// std::cout << 
			// 	" i1:" << index_1 << " i2:" << index_2 << 
			// 	" i3:" << index_3 << " i4:" << index_4 << 
			// 	" newVal:" << newVal << "->at:" << IxYp2pJ << 
			// 	" iters:" << i << ":" << j <<
			// std::endl;
		}
	}

	// Update array with new values.
	for (int i = 1; i <= XSIZE; ++i)
		for (int j = 1; j <= YSIZE; ++j)
		{
			int IxYp2pJ = i * YSIZE_PLUS_2 + j;
			h[IxYp2pJ] = h_new[IxYp2pJ];
		}
}

/*
 * Permanent heat source.
 */
void LaplaceHeat::heatSource() {

	int sizex, sizey;

	// Position 4 permanent heat sources.
	sizex = XSIZE / 4;
	sizey = YSIZE / 4;

	//std::cout << "HEAT_SOURCE" << std::endl;

	for (int i = 0; i < sizex; ++i)
		for (int j = 0; j < sizey; ++j){
			h[(sizex / 2 + i) * YSIZE_PLUS_2 + (sizey / 2 + j)] = HEAT_MAX;
			h[(sizex / 2 + i) * YSIZE_PLUS_2 + (3 * sizey - (sizey / 2) + j)] = HEAT_MAX;
			h[(3 * sizex - (sizex / 2) + i) * YSIZE_PLUS_2 + (sizey / 2 + j)] = HEAT_MAX;
			h[(3 * sizex - (sizex / 2) + i) * YSIZE_PLUS_2 + (3 * sizey - (sizey / 2) + j)] = HEAT_MAX;
			
			// int i1 = (sizex / 2 + i) * YSIZE_PLUS_2 + (sizey / 2 + j);
			// int i2 = (sizex / 2 + i) * YSIZE_PLUS_2 + (3 * sizey - (sizey / 2) + j);
			// int i3 = (3 * sizex - (sizex / 2) + i) * YSIZE_PLUS_2 + (sizey / 2 + j);
			// int i4 = (3 * sizex - (sizex / 2) + i) * YSIZE_PLUS_2 + (3 * sizey - (sizey / 2) + j);
			// h[i1] = HEAT_MAX;
			// h[i2] = HEAT_MAX;
			// h[i3] = HEAT_MAX;
			// h[i4] = HEAT_MAX;

			// std::cout << 
			// 	" i1:" << i1 << " i2:" << i2 << 
			// 	" i3:" << i3 << " i4:" << i4 << 
			// 	" i:" << i << " j:" << j <<
			// std::endl;
		}
}

/*
 *
 */
unsigned long LaplaceHeat::checksum() {

	unsigned long checksum = 0;

	for (int i = 0; i < XSIZE + 2; i++)
		for (int j = 0; j < YSIZE + 2; j++)
			checksum += (unsigned long)h[i * (YSIZE + 2) + j];

	return checksum;
}
