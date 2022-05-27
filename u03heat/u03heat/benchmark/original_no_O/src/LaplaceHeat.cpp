#include "LaplaceHeat.h"

/*
 *
 */
LaplaceHeat::LaplaceHeat(int xsize, int ysize, double heat_min, double heat_max, int steps, int niter)
		: XSIZE(xsize), YSIZE(ysize), HEAT_MIN(heat_min), HEAT_MAX(heat_max), STEPS(steps), NITER(niter) {

	// Additional 2 boundary cells to simplify calculation.
	h     = new double[(XSIZE + 2) * (YSIZE + 2)]; // Finite difference array.
	h_new = new double[(XSIZE + 2) * (YSIZE + 2)]; // Finite difference array (new values).
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
	for (int j = 1; j <= YSIZE; j++)
		for (int i = 1; i <= XSIZE; i++)
			h_new[i * (YSIZE + 2) + j] = 0.25 * (h[(i - 1) * (YSIZE + 2) + j] + h[(i + 1) * (YSIZE + 2) + j] + h[i  * (YSIZE + 2) + j - 1] + h[i * (YSIZE + 2) + j + 1]);

	// Update array with new values.
	for (int j = 1; j <= YSIZE; j++)
		for (int i = 1; i <= XSIZE; i++)
			h[i * (YSIZE + 2) + j] = h_new[i * (YSIZE + 2) + j];
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

	for (int j = 0; j < sizey; j++)
		for (int i = 0; i < sizex; i++) {
			h[(1 * XSIZE / 4 - (sizex / 2) + i) * (YSIZE + 2) + (1 * YSIZE / 4 - (sizey / 2) + j)] = HEAT_MAX;
			h[(1 * XSIZE / 4 - (sizex / 2) + i) * (YSIZE + 2) + (3 * YSIZE / 4 - (sizey / 2) + j)] = HEAT_MAX;
			h[(3 * XSIZE / 4 - (sizex / 2) + i) * (YSIZE + 2) + (1 * YSIZE / 4 - (sizey / 2) + j)] = HEAT_MAX;
			h[(3 * XSIZE / 4 - (sizex / 2) + i) * (YSIZE + 2) + (3 * YSIZE / 4 - (sizey / 2) + j)] = HEAT_MAX;
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
