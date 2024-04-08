#version 330 core

layout (triangles) in;
layout (line_strip, max_vertices = 3) out;

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


out vec3 mynormal;
out vec4 myvertex;
out vec2 myuv;

out vec3 m_ambiant;
out vec3 m_diffuse;
out vec3 m_specular;
out vec4 m_mra;
out vec3 tex_emissive_transparent;

void main() {
    for(int i = 0; i < gl_in.length(); i++){
        gl_Position = gl_in[i].gl_Position;

        myvertex = vs_out[0].v_myvertex;
        mynormal = vs_out[0].v_mynormal;
        myuv = vs_out[0].v_myuv;
        m_ambiant = vs_out[0].v_m_ambiant;
        m_diffuse = vs_out[0].v_m_diffuse;
        m_specular = vs_out[0].v_m_specular;
        m_mra = vs_out[0].v_m_mra;
        tex_emissive_transparent = vs_out[0].v_tex_emissive_transparent;

        EmitVertex();
    }






    EndPrimitive();
}