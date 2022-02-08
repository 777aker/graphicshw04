#version 400 core

// drawing the light source is very simple so why waste a bunch of calculations
// fake it for better effect that is much more efficient

//  Transformation matrices
uniform mat4 ProjectionMatrix;
uniform mat4 ModelViewMatrix;

//  Vertex attributes (input)
in vec4 Vertex;

// our sphere moves and this was the best idea I could think of
uniform vec4 Position;

void main() {
	// set transformed location
	gl_Position = ProjectionMatrix*ModelViewMatrix*Vertex;
}