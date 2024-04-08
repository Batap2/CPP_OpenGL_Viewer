#version 330 core

// Vertex array layout inputs to the vertex shader
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec3 in_m_diffuse;
layout (location = 4) in vec4 in_m_mra;
layout (location = 5) in vec3 in_tex_emissive_transparent;


// Uniform variables
uniform mat4 modelview;
uniform mat4 projection;
uniform int render_mode;

out VS_OUT
{
	vec3 v_mynormal;
	vec4 v_myvertex;
	vec2 v_myuv;
	vec3 v_m_ambiant;
	vec3 v_m_diffuse;
	vec3 v_m_specular;
	vec4 v_m_mra;
	vec3 v_tex_emissive_transparent;
} vs_out;

void main(){

	if(render_mode == 1){
		gl_Position = vec4(position, 1.0f);
	} else
	{
		gl_Position = projection * modelview * vec4(position, 1.0f);
	}

	vs_out.v_mynormal = normal;
	vs_out.v_myvertex = vec4(position, 1.0f);
	vs_out.v_myuv = texCoord;
	vs_out.v_m_diffuse = in_m_diffuse;
	vs_out.v_m_mra = in_m_mra;
	vs_out.v_tex_emissive_transparent = in_tex_emissive_transparent;
}