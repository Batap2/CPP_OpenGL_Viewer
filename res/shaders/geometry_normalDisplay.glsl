#version 330 core

layout (triangles) in;
layout (line_strip, max_vertices = 2) out;

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

out vec3 mynormal;
out vec4 myvertex;
out vec2 myuv;

out vec3 m_ambiant;
out vec3 m_diffuse;
out vec3 m_specular;
out vec4 m_mra;
out vec3 tex_emissive_transparent;

out vec3 barycenter;

out float noLighting;

void main() {

    for(int i = 0; i < gl_in.length(); i++)
    {
        gl_Position = projection * modelview * vec4(vs_out[i].v_myvertex.xyz, 1.0);
        m_diffuse = vec3(1,1,0);
        noLighting = 1;

        EmitVertex();

        gl_Position = projection * modelview * vec4(vs_out[i].v_myvertex.xyz + vs_out[i].v_mynormal * normalDisplayLength, 1.0);
        m_diffuse = vec3(1,1,0);
        noLighting = 1;

        EmitVertex();
        EndPrimitive();
    }
}