#include "LaplaceHeat.h"

#include <iostream>
#include <iomanip>
#include <chrono>

typedef std::chrono::high_resolution_clock Time;
typedef std::chrono::duration<double> dsec;

void usage(char *progname) {

	std::cout << "usage: " << progname << " [steps]" << std::endl;
	exit(1);
}

int main(int argc, char* argv[]) {

	LaplaceHeat heat(256, 256, 20.0, 100.0, 3000, 500);

	int niter = heat.NITER;

	// Check optional runtime parameters.
	if (argc == 2)
		if (sscanf(argv[1], "%d", &niter) != 1)
			usage(argv[0]);

	// Initialize data.
	for (int i = 0; i < heat.XSIZE + 2; i++)
		for (int j = 0; j < heat.YSIZE + 2; j++)
			heat.h[i * (heat.YSIZE + 2) + j] = heat.HEAT_MIN;

	// Add heat sources.
	heat.heatSource();

	// Iteration steps.
	auto ttotal = Time::now();
	auto t0 = ttotal;

	for (int t = 0; t < heat.STEPS; t++) {

		// Display every niter step.
		if (t % niter == (niter - 1)) {

			auto t1 = Time::now();
			dsec ds = (t1 - t0) / niter;
			std::cout << "time step " << (t+1) << " of " << heat.STEPS << ": " << std::setprecision(10) << ds.count() << " s per time step" << std::endl;
			t0 = Time::now();
		}

		// Compute new values.
		heat.update();

		// Permanent heat source.
		heat.heatSource();
	}

	dsec ds = Time::now() - ttotal;

	std::cout << "Total: " << ds.count() << " s" << std::endl;
	std::cout << "Checksum: " << heat.checksum() << std::endl;

	return 0;
}
