#ifndef INCLUDE_LAPLACEHEAT_H_
#define INCLUDE_LAPLACEHEAT_H_

class LaplaceHeat {
public:

	int XSIZE;
	int YSIZE;

	double HEAT_MIN;
	double HEAT_MAX;

	int STEPS;
	int NITER;

	LaplaceHeat(int xsize, int ysize, double heat_min, double heat_max, int steps, int niter);
	~LaplaceHeat();

	double* h; // Finite difference array.
	double* h_new; // Finite difference array (new values).
	int YSIZE_PLUS_2;

	void update();
	void heatSource();
	unsigned long checksum();

};

#endif /* INCLUDE_LAPLACEHEAT_H_ */
