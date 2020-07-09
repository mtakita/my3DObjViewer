#version 410 core

uniform mat4 model_matrix;
uniform mat4 projection_matrix;

layout ( location = 0 ) in vec4 vPosition;
layout ( location = 1 ) in vec2 texCoords;
layout ( location = 2 ) in vec3 normal;

void
main()
{
	// Swells a little.
	vec4 vPosSwell = vPosition + vec4( normal * 0.32, 0.0);

	vec4 pos = vPosSwell * vec4(1.0, 1.0, 1.0, 1.0) * model_matrix;
	gl_Position = pos * projection_matrix;

}