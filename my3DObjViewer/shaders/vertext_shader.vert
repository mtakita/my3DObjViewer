#version 410 core

uniform mat4 model_matrix;
uniform mat4 projection_matrix;

layout ( location = 0 ) in vec4 vPosition;
layout ( location = 1 ) in vec2 texCoords;
layout ( location = 2 ) in vec3 normal;

out vec4 world_space_position;

out vec3 vs_fs_normal;
out vec2 vs_tex_coords;


void
main()
{
	vec4 pos = (model_matrix * (vPosition * vec4(1.0, 1.0, 1.0, 1.0)));
	world_space_position = pos;
    vs_fs_normal = normalize((model_matrix * vec4(normal, 0.0)).xyz);
	gl_Position = projection_matrix * pos;

	vs_tex_coords = texCoords;

}