#version 430
layout(local_size_x = 1, local_size_y = 1) in;
layout(binding = 2) uniform atomic_uint atominc;

struct Particle {
	vec2 position;
	vec2 velocity;
};

layout (std430, binding=4) buffer particle_buf
{ 
	Particle particles[500];
};

void main() { 
	uint index = gl_GlobalInvocationID.x;

	particles[index].position.y = 250;
}
