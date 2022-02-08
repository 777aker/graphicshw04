#version 400 core

//  Input from previous shader
in vec4 FrontColor;
in vec2 Texcoord;

in vec3 FragView;
in vec3 FragLight;
in vec3 FragNormal;
in vec4 FragAmbient;

in vec3 ModelPos;

uniform vec4 Specular;
uniform vec4 Diffuse;

//  Fragment color
out vec4 Fragcolor;

// texture 
uniform sampler2D tex;

// time passed
uniform float time;

vec4 blinn() {
	// N is object normal
	vec3 N = normalize(FragNormal);
	// light vector 
	vec3 L = normalize(FragLight);
	// view vector
	vec3 V = normalize(FragView);

	// emission and ambient color 
	vec4 color = FragAmbient;

	// diffuse light 
	float Id = dot(L,N);
	if(Id>0.0) {
		// add diffuse 
		color += Id*Diffuse*FrontColor;
		// half vectors 
		vec3 H = normalize(V+L);
		// specular 
		float Is = dot(H,N);
		if (Is>0.0) color += pow(Is, 8.0) * Specular;
	}
	return color;
}

void main()
{
   Fragcolor = blinn()*texture(tex,Texcoord);
}
