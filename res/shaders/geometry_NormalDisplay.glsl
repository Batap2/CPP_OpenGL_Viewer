#version 330 core

layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in VS_OUT
{
    vec3 v_mynormal;
    vec4 v_myvertex;
    vec2 v_myuv;
    vec3 v_m_ambiant;
    vec3 v_m_diffuse;
    vec3 v_m_specular;
    vec4 v_m_mra;
    vec3 v_tex_emissive_transparent;
} vs_out[];

uniform mat4 modelview;
uniform mat4 projection;
uniform float normalDisplayLength;

void main() {

    for(int i = 0; i < gl_in.length(); i++)
    {
        gl_Position = projection * modelview * vec4(vs_out[i].v_myvertex.xyz, 1.0);

        EmitVertex();

        gl_Position = projection * modelview * vec4(vs_out[i].v_myvertex.xyz + vs_out[i].v_mynormal * normalDisplayLength, 1.0);

        EmitVertex();
        EndPrimitive();
    }
}