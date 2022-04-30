#include "FluidCube.h"

#include <iostream>
#include <iomanip>

int main(int argc, char* argv[]) {

	FluidCube* fc = new FluidCube();

	fc->createCube(32, 100.0, 200.0, 1.0);

	for (int z = 0; z < fc->cube->size; ++z)
	{
		for (int y = 0; y < fc->cube->size; ++y)
		{
			for (int x = 0; x < fc->cube->size; ++x)
			{
				fc->addDensity(x, y, z, 0.2);
				fc->addVelocity(x, y, z, 1.0, 1.0, 1.0);
			}
		}
	}

	fc->addDensity(2, 0, 0, 40.0);
	fc->addVelocity(2, 0, 0, 10.0, 30.0, 24.0);

//	fc->printCube();

	for (int i = 0; i < 50; ++i)
	{
//		std::cout << "ITERATION " << i << std::endl;

		fc->step();

//		fc->printCube();
	}

	// Checksum = 1280.561359
	std::cout << std::setprecision(10) << "Checksum: " << fc->checksum() << std::endl;;

	fc->freeCube();

	delete fc;
}
