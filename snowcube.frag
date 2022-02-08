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
		// specular is a bit much maybe did it wrong? or shininess?
		// maybe decrease specular or shininess for other things
		if (Is>0.0) color += pow(Is, 8.0) * Specular;
	}
	return color;
}

void main()
{
	// if you want normal per pixel remove everything below and just do
	// Fragcolor = blinn() * texture(tex, Texcoord);
	// build noise 
	// add time to y so snow falling effect 
	float noise = fract(sin(dot(floor((Texcoord.xy+vec2(0,time*.25))*10), vec2(12.9898, 78.233)))*43758.5453);
	// clamp it so it looks cool
	noise = clamp(step(.8, noise) * (ModelPos.y+1), 0, 1)*5;
	// construct base color + snow 
	vec4 color = vec4(noise+.2, noise+.2, noise+.4, 1);
	// color fade as you go down;
    color *= ModelPos.y+1;
	// make top a cloud 
	color += step(1-ModelPos.y, .2)*4;
	// make snow build up on ground 
	if(Texcoord.y < .5)
		color += step((1-abs(ModelPos.y)*time*.25), .2)*4;
	// make bottom invisible bc otherwise looks silly 
	color *= step(-ModelPos.y-.8, 0);
	if(ModelPos.y <= -.8)
		discard;
	// actually color 
	Fragcolor = blinn() * color;
}
