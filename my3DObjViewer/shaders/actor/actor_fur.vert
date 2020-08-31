#version 410 core

uniform mat4 model_matrix;
uniform mat4 projection_matrix;

layout ( location = 0 ) in vec4 vPosition;
layout ( location = 1 ) in vec2 texCoords;
layout ( location = 2 ) in vec3 normal;

out vec4 world_space_position;

out vec3 vs_fs_normal;

out VS_OUT{
	vec4 vPosition;
	vec2 vs_tex_coords;
	vec3 normal;
}vs_out;


void
main()
{
	vs_out.vPosition = vPosition;
	vs_out.vs_tex_coords = texCoords;
	vs_out.normal = normal;
}