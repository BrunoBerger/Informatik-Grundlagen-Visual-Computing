#version 430
layout(local_size_x = 1, local_size_y = 1) in;
layout(rgba32f) uniform image2D img_output;

struct Particle {
	vec2 position;
	vec2 velocity;
};

layout (std430, binding=3) buffer particle_buf
{ 
	Particle particles[500];
};

void main() {
	uint index = gl_GlobalInvocationID.x;
	if (index < 500) {




	// pixel = vec4(0.0, 1.0, 0.0, 1.0);
		ivec2 new_pixel_pos = ivec2(
			particles[index].position.x, 
			particles[index].position.y
		);
		vec4 new_pixel_col = vec4(1.0, 1.0, 0.0, 1.0);
		//pixel = vec4(0.6, 0.6, 0.1, 1.0);
		imageStore(img_output, new_pixel_pos, new_pixel_col);
	}
}
