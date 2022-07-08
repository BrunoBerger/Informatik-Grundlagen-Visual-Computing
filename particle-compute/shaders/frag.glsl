#version 330 core

layout(location = 0) out vec3 outColor;


uniform sampler2D img_input;

in vec3 oColor;

void main() {
    vec4 c = texture(img_input, oColor.xy);
    outColor = c.xyz;
}