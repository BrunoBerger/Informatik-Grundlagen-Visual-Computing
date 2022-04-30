#ifndef INCLUDE_FLUIDCUBE_H_
#define INCLUDE_FLUIDCUBE_H_

class FluidCube {

public:

	struct Cube {
	    int size;
	    double dt;
	    double diff;
	    double visc;

	    double *s;
	    double *density;

	    double *Vx;
	    double *Vy;
	    double *Vz;

	    double *Vx0;
	    double *Vy0;
	    double *Vz0;
	};

	void createCube(int size, int diffusion, int viscosity, double dt);
	void freeCube();
	void set_bnd(int b, double *x, int N);
	void lin_solve(int b, double *x, double *x0, double a, double c, int iter, int N);
	void diffuse (int b, double *x, double *x0, double diff, double dt, int iter, int N);
	void advect(int b, double *d, double *d0,  double *velocX, double *velocY, double *velocZ, double dt, int N);
	void project(double *velocX, double *velocY, double *velocZ, double *p, double *div, int iter, int N);
	void step();
	void addDensity(int x, int y, int z, double amount);
	void addVelocity(int x, int y, int z, double amountX, double amountY, double amountZ);
	void print();
	double checksum();

	Cube* cube = nullptr;
};

#endif /* INCLUDE_FLUIDCUBE_H_ */
