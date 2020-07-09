#version 410 core

layout ( triangles) in;
layout ( triangle_strip, max_vertices = 3 ) out;

in VS_OUT{
	vec2 vs_tex_coords;
}tes_out[];

out VS_OUT{
	vec2 vs_tex_coords;
}geo_out;

void main( void ){

	int n;

	for( n = 0; n < gl_in.length(); n++ ){

		gl_Position = gl_in[0].gl_Position;
		geo_out.vs_tex_coords = tes_out[n].vs_tex_coords;

		EmitVertex();
	}

	EmitVertex();
}
