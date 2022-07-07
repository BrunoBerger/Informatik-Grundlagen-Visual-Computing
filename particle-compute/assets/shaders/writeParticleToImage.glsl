#version 430
layout(local_size_x = 1, local_size_y = 1) in;
layout(rgba32f) uniform image2D img_output;

struct Particle {
	vec2 position;
	vec2 velocity;
	double dist;
};

layout (std430, binding=3) buffer particle_buf
{ 
	Particle particles[];
};

void main() {
	uint index = gl_GlobalInvocationID.x;
	ivec2 new_pixel_pos = ivec2(
		particles[index].position.x, 
		particles[index].position.y
	);
	vec4 new_pixel_col = vec4(
		1.0,
		particles[index].dist,
		0.8, 
		1.0
	);
	imageStore(img_output, new_pixel_pos, new_pixel_col);
}
