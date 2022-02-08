#version 400 core 

// light source color
uniform vec4 LightColor;

// fragment color
out vec4 Fragcolor;

void main() {
	Fragcolor = LightColor;
}