#version 430
layout(local_size_x = 1, local_size_y = 1) in;
layout(binding = 2) uniform atomic_uint atominc;

struct Particle {
	vec2 position;
	vec2 velocity;
};

layout (std430, binding=4) buffer particle_buf
{ 
	Particle particles[];
};

int IMG_WIDTH = 640;
int IMG_HEIGHT = 480;


void main() { 
	uint index = gl_GlobalInvocationID.x;
	vec2 pos = particles[index].position;

	if (pos.x > IMG_WIDTH || pos.x < 0)
		particles[index].velocity.x *= -1;
	if (pos.y > IMG_HEIGHT || pos.y < 0)
		particles[index].velocity.y *= -1;

	particles[index].position.x += 0.1 * particles[index].velocity.x;
	particles[index].position.y += 0.1 * particles[index].velocity.y;
	
}
