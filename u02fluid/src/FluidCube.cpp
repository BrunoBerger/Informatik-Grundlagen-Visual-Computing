#include "FluidCube.h"

#include <cstdlib>
#include <cmath>
#include <iostream>

#define IX(x, y, z) ((x) + (y) * N + (z) * N * N)

void FluidCube::createCube(int size, int diffusion, int viscosity, double dt)
{
    cube = (Cube*)malloc(sizeof(*cube));
    int N = size;

    cube->size = size;
    cube->dt = dt;
    cube->diff = diffusion;
    cube->visc = viscosity;

    cube->s = (double*)calloc(N * N * N, sizeof(double));
    cube->density = (double*)calloc(N * N * N, sizeof(double));

    cube->Vx = (double*)calloc(N * N * N, sizeof(double));
    cube->Vy = (double*)calloc(N * N * N, sizeof(double));
    cube->Vz = (double*)calloc(N * N * N, sizeof(double));

    cube->Vx0 = (double*)calloc(N * N * N, sizeof(double));
    cube->Vy0 = (double*)calloc(N * N * N, sizeof(double));
    cube->Vz0 = (double*)calloc(N * N * N, sizeof(double));
}

void FluidCube::freeCube()
{
    free(cube->s);
    free(cube->density);

    free(cube->Vx);
    free(cube->Vy);
    free(cube->Vz);

    free(cube->Vx0);
    free(cube->Vy0);
    free(cube->Vz0);

    free(cube);
    cube = nullptr;
}


void FluidCube::set_bnd(int b, double *x, int N)
{
    for(int j = 1; j < N - 1; j++) {
        for(int i = 1; i < N - 1; i++) {
            x[IX(i, j, 0  )] = b == 3 ? -x[IX(i, j, 1  )] : x[IX(i, j, 1  )];
            x[IX(i, j, N-1)] = b == 3 ? -x[IX(i, j, N-2)] : x[IX(i, j, N-2)];
        }
    }
    for(int k = 1; k < N - 1; k++) {
        for(int i = 1; i < N - 1; i++) {
            x[IX(i, 0  , k)] = b == 2 ? -x[IX(i, 1  , k)] : x[IX(i, 1  , k)];
            x[IX(i, N-1, k)] = b == 2 ? -x[IX(i, N-2, k)] : x[IX(i, N-2, k)];
        }
    }
    for(int k = 1; k < N - 1; k++) {
        for(int j = 1; j < N - 1; j++) {
            x[IX(0  , j, k)] = b == 1 ? -x[IX(1  , j, k)] : x[IX(1  , j, k)];
            x[IX(N-1, j, k)] = b == 1 ? -x[IX(N-2, j, k)] : x[IX(N-2, j, k)];
        }
    }

    x[IX(0, 0, 0)]       = 0.33f * (x[IX(1, 0, 0)]
                                  + x[IX(0, 1, 0)]
                                  + x[IX(0, 0, 1)]);
    x[IX(0, N-1, 0)]     = 0.33f * (x[IX(1, N-1, 0)]
                                  + x[IX(0, N-2, 0)]
                                  + x[IX(0, N-1, 1)]);
    x[IX(0, 0, N-1)]     = 0.33f * (x[IX(1, 0, N-1)]
                                  + x[IX(0, 1, N-1)]
                                  + x[IX(0, 0, N)]);
    x[IX(0, N-1, N-1)]   = 0.33f * (x[IX(1, N-1, N-1)]
                                  + x[IX(0, N-2, N-1)]
                                  + x[IX(0, N-1, N-2)]);
    x[IX(N-1, 0, 0)]     = 0.33f * (x[IX(N-2, 0, 0)]
                                  + x[IX(N-1, 1, 0)]
                                  + x[IX(N-1, 0, 1)]);
    x[IX(N-1, N-1, 0)]   = 0.33f * (x[IX(N-2, N-1, 0)]
                                  + x[IX(N-1, N-2, 0)]
                                  + x[IX(N-1, N-1, 1)]);
    x[IX(N-1, 0, N-1)]   = 0.33f * (x[IX(N-2, 0, N-1)]
                                  + x[IX(N-1, 1, N-1)]
                                  + x[IX(N-1, 0, N-2)]);
    x[IX(N-1, N-1, N-1)] = 0.33f * (x[IX(N-2, N-1, N-1)]
                                  + x[IX(N-1, N-2, N-1)]
                                  + x[IX(N-1, N-1, N-2)]);
}

void FluidCube::lin_solve(int b, double *x, double *x0, double a, double c, int iter, int N)
{
    float cRecip = 1.0 / c;
    for (int k = 0; k < iter; k++) {
        for (int m = 1; m < N - 1; m++) {
            for (int j = 1; j < N - 1; j++) {
                for (int i = 1; i < N - 1; i++) {
                    x[IX(i, j, m)] =
                        (x0[IX(i, j, m)]
                            + a*(    x[IX(i+1, j  , m  )]
                                    +x[IX(i-1, j  , m  )]
                                    +x[IX(i  , j+1, m  )]
                                    +x[IX(i  , j-1, m  )]
                                    +x[IX(i  , j  , m+1)]
                                    +x[IX(i  , j  , m-1)]
                           )) * cRecip;
                }
            }
        }
        set_bnd(b, x, N);
    }
}

void FluidCube::diffuse(int b, double *x, double *x0, double diff, double dt, int iter, int N)
{
	double a = dt * diff * (N - 2) * (N - 2);
	lin_solve(b, x, x0, a, 1 + 6 * a, iter, N); //
}

void FluidCube::advect(int b, double *d, double *d0,  double *velocX, double *velocY, double *velocZ, double dt, int N)
{
	double i0, i1, j0, j1, k0, k1;

	double dtx = dt * (N - 2);
	double dty = dt * (N - 2);
	double dtz = dt * (N - 2);

	double s0, s1, t0, t1, u0, u1;
	double tmp1, tmp2, tmp3, x, y, z;

	double Nd = N;
	double id, jd, kd;
    int i, j, k;

    for(k = 1, kd = 1; k < N - 1; k++, kd++) {
        for(j = 1, jd = 1; j < N - 1; j++, jd++) {
            for(i = 1, id = 1; i < N - 1; i++, id++) {
                tmp1 = dtx * velocX[IX(i, j, k)];
                tmp2 = dty * velocY[IX(i, j, k)];
                tmp3 = dtz * velocZ[IX(i, j, k)];
                x    = id - tmp1;
                y    = jd - tmp2;
                z    = kd - tmp3;

                if(x < 0.5f) x = 0.5f;
                if(x > Nd + 0.5f) x = Nd + 0.5f;
                i0 = floor(x);
                i1 = i0 + 1.0f;
                if(y < 0.5f) y = 0.5f;
                if(y > Nd + 0.5f) y = Nd + 0.5f;
                j0 = floor(y);
                j1 = j0 + 1.0f;
                if(z < 0.5f) z = 0.5f;
                if(z > Nd + 0.5f) z = Nd + 0.5f;
                k0 = floor(z);
                k1 = k0 + 1.0f;

                s1 = x - i0;
                s0 = 1.0f - s1;
                t1 = y - j0;
                t0 = 1.0f - t1;
                u1 = z - k0;
                u0 = 1.0f - u1;

                int i0i = i0;
                int i1i = i1;
                int j0i = j0;
                int j1i = j1;
                int k0i = k0;
                int k1i = k1;

                d[IX(i, j, k)] =

                    s0 * ( t0 * (u0 * d0[IX(i0i, j0i, k0i)]
                                +u1 * d0[IX(i0i, j0i, k1i)])
                        +( t1 * (u0 * d0[IX(i0i, j1i, k0i)]
                                +u1 * d0[IX(i0i, j1i, k1i)])))
                   +s1 * ( t0 * (u0 * d0[IX(i1i, j0i, k0i)]
                                +u1 * d0[IX(i1i, j0i, k1i)])
                        +( t1 * (u0 * d0[IX(i1i, j1i, k0i)]
                                +u1 * d0[IX(i1i, j1i, k1i)])));
            }
        }
    }
    set_bnd(b, d, N);
}

void FluidCube::project(double *velocX, double *velocY, double *velocZ, double *p, double *div, int iter, int N)
{
    for (int k = 1; k < N - 1; k++) {
        for (int j = 1; j < N - 1; j++) {
            for (int i = 1; i < N - 1; i++) {
                div[IX(i, j, k)] = -0.5f*(
                         velocX[IX(i+1, j  , k  )]
                        -velocX[IX(i-1, j  , k  )]
                        +velocY[IX(i  , j+1, k  )]
                        -velocY[IX(i  , j-1, k  )]
                        +velocZ[IX(i  , j  , k+1)]
                        -velocZ[IX(i  , j  , k-1)]
                    )/N;
                p[IX(i, j, k)] = 0;
            }
        }
    }
    set_bnd(0, div, N);
    set_bnd(0, p, N);
    lin_solve(0, p, div, 1, 6, iter, N);

    for (int k = 1; k < N - 1; k++) {
        for (int j = 1; j < N - 1; j++) {
            for (int i = 1; i < N - 1; i++) {
                velocX[IX(i, j, k)] -= 0.5f * (  p[IX(i+1, j, k)]
                                                -p[IX(i-1, j, k)]) * N;
                velocY[IX(i, j, k)] -= 0.5f * (  p[IX(i, j+1, k)]
                                                -p[IX(i, j-1, k)]) * N;
                velocZ[IX(i, j, k)] -= 0.5f * (  p[IX(i, j, k+1)]
                                                -p[IX(i, j, k-1)]) * N;
            }
        }
    }
    set_bnd(1, velocX, N);
    set_bnd(2, velocY, N);
    set_bnd(3, velocZ, N);
}

void FluidCube::step()
{
    int N           = cube->size;
    
    double visc     = cube->visc;
    double diff     = cube->diff;
    double dt       = cube->dt;
    double *Vx      = cube->Vx;
    double *Vy      = cube->Vy;
    double *Vz      = cube->Vz;
    double *Vx0     = cube->Vx0;
    double *Vy0     = cube->Vy0;
    double *Vz0     = cube->Vz0;
    double *s       = cube->s;
    double *density = cube->density;

    diffuse(1, Vx0, Vx, visc, dt, 4, N);
    diffuse(2, Vy0, Vy, visc, dt, 4, N);
    diffuse(3, Vz0, Vz, visc, dt, 4, N);

    project(Vx0, Vy0, Vz0, Vx, Vy, 4, N);

    advect(1, Vx, Vx0, Vx0, Vy0, Vz0, dt, N);
    advect(2, Vy, Vy0, Vx0, Vy0, Vz0, dt, N);
    advect(3, Vz, Vz0, Vx0, Vy0, Vz0, dt, N);

    project(Vx, Vy, Vz, Vx0, Vy0, 4, N);

    diffuse(0, s, density, diff, dt, 4, N);
    advect(0, density, s, Vx, Vy, Vz, dt, N);
}

void FluidCube::addDensity(int x, int y, int z, double amount)
{
    int N = cube->size;
    cube->density[IX(x, y, z)] += amount;
}

void FluidCube::addVelocity(int x, int y, int z, double amountX, double amountY, double amountZ)
{
    int N = cube->size;
    int index = IX(x, y, z);

    cube->Vx[index] += amountX;
    cube->Vy[index] += amountY;
    cube->Vz[index] += amountZ;
}

void FluidCube::print()
{
	int N = cube->size;

	for (int z = 0; z < cube->size; ++z)
	{
		for (int y = 0; y < cube->size; ++y)
		{
			for (int x = 0; x < cube->size; ++x)
			{
				std::cout << "(" << x << "," << y << "," << z << ") => [" << IX(x, y, z) << "]"
						<< " s=" << cube->s[IX(x, y, z)]
						<< " density=" << cube->density[IX(x, y, z)]
						<< " Vx=" << cube->Vx[IX(x, y, z)]
						<< " Vy=" << cube->Vy[IX(x, y, z)]
						<< " Vz=" << cube->Vz[IX(x, y, z)]
						<< " Vx0=" << cube->Vx0[IX(x, y, z)]
						<< " Vy0=" << cube->Vy0[IX(x, y, z)]
						<< " Vz0=" << cube->Vz0[IX(x, y, z)] << std::endl;
			}
		}
	}
}

double FluidCube::checksum()
{
	int N = cube->size;

	double checksum = 0.0;

	for (int z = 0; z < cube->size; ++z)
	{
		for (int y = 0; y < cube->size; ++y)
		{
			for (int x = 0; x < cube->size; ++x)
			{
				checksum += cube->s[IX(x, y, z)]
									+ cube->density[IX(x, y, z)]
									+ cube->Vx[IX(x, y, z)]
									+ cube->Vy[IX(x, y, z)]
									+ cube->Vz[IX(x, y, z)]
									+ cube->Vx0[IX(x, y, z)]
									+ cube->Vy0[IX(x, y, z)]
									+ cube->Vz0[IX(x, y, z)];
			}
		}
	}

	return checksum;
}
