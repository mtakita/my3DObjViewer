#version 410 core

uniform mat4 model_matrix;
uniform mat4 projection_matrix;

layout ( location = 0 ) in vec4 vPosition;
layout ( location = 1 ) in vec2 texCoords;
layout ( location = 2 ) in vec3 normal;

out vec4 world_space_position;

out vec3 vs_fs_normal;

out VS_OUT{
	vec2 vs_tex_coords;
}vs_out;


void
main()
{
	vec4 pos = vPosition * vec4(1.0, 1.0, 1.0, 1.0) * model_matrix;
	world_space_position = pos;
    vs_fs_normal = normalize((vec4(normal, 0.0) * model_matrix ).xyz);
	gl_Position = pos * projection_matrix;

	vs_out.vs_tex_coords = texCoords;

}