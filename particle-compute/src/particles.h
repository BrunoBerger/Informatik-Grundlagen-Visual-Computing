#include <vector>

#include "util.h"

struct Particle{
	int pos;
};

class ParticleSystem 
{
public:
	//Particle* Particles;
	std::vector<Particle> Particles;
	ParticleSystem(int N) {
		//Particles = new Particle[sizeof(Particle) * N];
		for (auto i : Particles) {
			Particles.push_back(Particle());
			i.pos = 1;
		}
	}
	void test() {
		for (auto i : Particles) {
			std::cout << "Particle:" << i.pos << "\n";
		}
	}



};
