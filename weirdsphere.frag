//  Fragment shader
#version 400 core

//  Fragment color
out vec4 Fragcolor;

in vec3 Light;

in vec4 ModelPos;

uniform float time;

//http://www.science-and-fiction.org/rendering/noise.html
float rand3D(in vec3 co) {
    return fract(sin(dot(co.xyz, vec3(12.9898, 78.233, 144.7272))) * 43758.5453);
}

void main()
{
	float dis = clamp(1-distance(Light,ModelPos.xyz)*30/75, 0, 5)*2.5;
	vec3 timeyyme = vec3(-time*2, time, -time);
	vec4 color = vec4(dis*2, dis*2, dis*2, 1);
	color += (rand3D(floor((ModelPos.xyz-timeyyme/5)*10)) <= 0.2) ? 1 : 0;
	Fragcolor = color;
}