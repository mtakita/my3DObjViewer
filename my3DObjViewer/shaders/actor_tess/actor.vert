#version 410 core

uniform mat4 model_matrix;
uniform mat4 projection_matrix;

layout ( location = 0 ) in vec4 vPosition;
layout ( location = 1 ) in vec2 texCoords;
layout ( location = 2 ) in vec3 normal;

//out VS_OUT{
//	vec2 vs_tex_coords;
//}vs_out;

out vec2 vs_tex_coords;

void
main()
{
	gl_Position = vPosition;

	vs_tex_coords = texCoords;

}