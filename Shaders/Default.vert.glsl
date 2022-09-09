#version 460

layout(location = 0) in vec2 in_Pos;

void main()
{
	gl_Position = vec4(in_Pos, 0.0, 1.0);
}
 