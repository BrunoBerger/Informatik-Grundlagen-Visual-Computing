#version 430
layout(local_size_x = 1, local_size_y = 1) in;
uniform vec2 cursorPos;

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
vec2 blackHolePos = vec2(IMG_WIDTH/2, IMG_HEIGHT/2);
float blackHoleStr = 0.00001;
float simSpeed = 0.1;

void main() { 
	uint index = gl_GlobalInvocationID.x;
	vec2 pos = particles[index].position;

	// Gravitational pull
	blackHolePos = cursorPos; // toggle following the cursor
	vec2 force = (blackHolePos - pos) * blackHoleStr;
	particles[index].velocity += force;

	// Keep particles in frame
	if (pos.x > IMG_WIDTH || pos.x < 0)
		particles[index].velocity.x *= -1;
	if (pos.y > IMG_HEIGHT || pos.y < 0)
		particles[index].velocity.y *= -1;

	// Some slowdown
	particles[index].velocity *= 0.9999;

	// Apply current velocity
	particles[index].position += simSpeed * particles[index].velocity;
}
