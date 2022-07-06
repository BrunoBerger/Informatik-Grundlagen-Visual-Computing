#version 430
layout(local_size_x = 8, local_size_y = 4) in;
layout(rgba32f) uniform image2D img_output;

layout(binding = 2) uniform atomic_uint atominc;
uniform float testVar;

struct Particle {
	vec2 position;
	vec2 velocity;
};

layout (std430, binding=3) buffer particle_buf
{ 
	Particle particles[100];
};

void main() {
	// base pixel colour for image
	vec4 pixel = vec4(0.1, 0.1, 0.1, 1.0);

	// get index in global work group i.e x,y position
	ivec2 pixel_coords = ivec2(gl_GlobalInvocationID.xy);
	if ((pixel_coords.x / 4 ) % 2 == 0) {
		//pixel = vec4(0.0, 1.0, 0.0, 1.0);
	}
	//imageStore(img_output, pixel_coords, pixel);
  
	uint index = gl_GlobalInvocationID.x;
	if (index < 100) {
	// pixel = vec4(0.0, 1.0, 0.0, 1.0);
		ivec2 new_pixel_pos = ivec2(
			particles[index].position.x, 
			particles[index].position.y
		);
		vec4 new_pixel_col = vec4(1.0, 0.0, 0.0, 1.0);
		//pixel = vec4(0.6, 0.6, 0.1, 1.0);
		imageStore(img_output, new_pixel_pos, new_pixel_col);
	}
	
	//imageStore(img_output, pixel_coords, pixel);
	

}
