#version 400 core

//  Transformation matrices
uniform mat4 ProjectionMatrix;
uniform mat4 ModelViewMatrix;

//  Vertex attributes (input)
in vec4 Vertex;

//  Output to next shader
out vec4 ModelPos;

uniform vec4 Position;

out vec3 Light;

void main()
{	
   //  Pass colors to fragment shader (will be interpolated)
   
   Light = vec3(Position);

   ModelPos = Vertex;

   //  Set transformed vertex location
   gl_Position =  ProjectionMatrix * ModelViewMatrix * Vertex;
}