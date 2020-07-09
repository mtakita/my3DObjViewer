#version 410 core

uniform mat4 projection_matrix;
uniform mat4 model_matrix;

layout ( location = 0 ) in vec4 vPosition;
layout ( location = 1 ) in vec4 vColor;

out vec4 vsColor;

void
main()
{
	gl_Position = (vPosition * vec4(1.0, 1.0, 1.0, 1.0) ) * model_matrix * projection_matrix;
	vsColor = vColor;
}