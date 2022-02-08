#version 400 core

//  Transformation matrices
uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;

//  Light propeties
uniform float fov;
uniform vec4 Global;
uniform vec4 Ambient;
uniform vec4 Position;

//  Vertex attributes (input)
in vec4 Vertex;
in vec3 Normal;
in vec4 Color;
in vec2 Texture;

//  Output to next shader
out vec4 FrontColor;
out vec2 Texcoord;

// for per pixel lighting in frag shader cause I like per pixel lighting
out vec3 FragView;
out vec3 FragLight;
out vec3 FragNormal;
out vec4 FragAmbient;
out vec4 Specular;

out vec3 ModelPos;

void main()
{	
	// vertex position for some calculations in frag shader
	ModelPos = Vertex.xyz;
   // lighting values for frag shader
   // vertex in modelview coord
   vec3 P = vec3(ModelViewMatrix * Vertex);
   // light Position
   FragLight = vec3(ViewMatrix * Position) - P;
   // normal 
   FragNormal = NormalMatrix * Normal;
   // eye position 
   FragView = -P;
   // ambient color 
   FragAmbient = (Global + Ambient) * Color;
   // texture 
   Texcoord = Texture;
   // object Color
   FrontColor = Color;
   //  Set transformed vertex location
   gl_Position =  ProjectionMatrix * ModelViewMatrix * Vertex;
}
