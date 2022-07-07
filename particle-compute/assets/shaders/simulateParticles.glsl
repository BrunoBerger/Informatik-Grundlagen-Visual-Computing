#version 430
layout(local_size_x = 1, local_size_y = 1) in;
uniform vec2 cursorPos;

struct Particle {
	vec2 position;
	vec2 velocity;
	double dist;
};

layout (std430, binding=4) buffer particle_buf
{ 
	Particle particles[];
};

int IMG_WIDTH = 640;
int IMG_HEIGHT = 480;


vec2 blackHolePos = vec2(IMG_WIDTH/2, IMG_HEIGHT/2);
float blackHoleStr = 0.00001;
float simSpeed = 0.1;

void main() { 
	uint index = gl_GlobalInvocationID.x;
	vec2 pos = particles[index].position;

	// Gravitational pull
	blackHolePos = cursorPos;
	vec2 distV = blackHolePos - pos;
	vec2 force = distV * blackHoleStr;
	particles[index].velocity += force;

	// Keep particles in frame
	if (pos.x > IMG_WIDTH || pos.x < 0)
		particles[index].velocity.x *= -1;
	if (pos.y > IMG_HEIGHT || pos.y < 0)
		particles[index].velocity.y *= -1;

	// Slowdown and dist for color
	particles[index].velocity *= 0.99995;
	particles[index].dist = length(distV)/300;

	// Apply current velocity
	particles[index].position += simSpeed * particles[index].velocity;
}
