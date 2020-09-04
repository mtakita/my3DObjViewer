#version 410 core

layout ( triangles) in;
layout ( triangle_strip, max_vertices = 120 ) out;

uniform mat4 model_matrix;
uniform mat4 projection_matrix;
uniform float fur_layers = 30.0f;
uniform float fur_depth = 0.05f;

in VS_OUT{
	vec4 vPosition;
	vec2 vs_tex_coords;
	vec3 normal;
}tes_out[];

out GS_OUT{
	vec2 vs_tex_coords;
	vec3 normal;
	flat float fur_strength;
}geo_out;

void main( void ){

	int n;
	int layer;
	vec4 position;

	float fur_disp_delta = 1.0f / fur_layers;
	float d = 0;

	for( layer = 0; layer < fur_layers; layer++ ){
		for( n = 0; n < gl_in.length(); n++ ){

			position = tes_out[n].vPosition + vec4( layer * fur_depth * tes_out[n].normal, 0.0f );
			gl_Position = position * model_matrix * projection_matrix;

			geo_out.normal = tes_out[n].normal;
			geo_out.vs_tex_coords = tes_out[n].vs_tex_coords;
			geo_out.fur_strength = (1.0f - d );

			EmitVertex();
		}

		d += fur_disp_delta;
		EndPrimitive();
	}

}
