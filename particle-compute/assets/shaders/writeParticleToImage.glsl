#version 430
layout(local_size_x = 10, local_size_y = 10) in;
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
	// pixel = vec4(0.0, 1.0, 0.0, 1.0);
	pixel = vec4(
		particles[0].position.x, 
		particles[0].position.y, 
		0.0, 1.0
	);
	//atomicCounterIncrement(atominc);
  }

  // output to a specific pixel in the image
  imageStore(img_output, pixel_coords, pixel);
}
