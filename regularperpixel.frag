#version 400 core

//  Input from previous shader
in vec4 FrontColor;
in vec2 Texcoord;

//  Fragment color
out vec4 Fragcolor;

//  Texture
uniform sampler2D tex;

void main()
{
   Fragcolor = FrontColor*texture(tex,Texcoord);
}
