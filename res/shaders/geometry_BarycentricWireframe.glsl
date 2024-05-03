#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 6) out;

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
uniform vec2 screenSize;

out vec3 mynormal;
out vec4 myvertex;
out vec2 myuv;
out vec3 m_ambiant;
out vec3 m_diffuse;
out vec3 m_specular;
out vec4 m_mra;
out vec3 tex_emissive_transparent;

noperspective out vec3 distFromEdge;


void passData()
{
    mynormal = vs_out[0].v_mynormal;
    myvertex = vs_out[0].v_myvertex;
    myuv = vs_out[0].v_myuv;
    m_ambiant = vs_out[0].v_m_ambiant;
    m_diffuse = vs_out[0].v_m_diffuse;
    m_specular = vs_out[0].v_m_specular;
    m_mra = vs_out[0].v_m_mra;
    tex_emissive_transparent = vs_out[0].v_tex_emissive_transparent;
}

void main() {
    vec2 p0 = screenSize * gl_in[0].gl_Position.xy / gl_in[0].gl_Position.w;
    vec2 p1 = screenSize * gl_in[1].gl_Position.xy / gl_in[1].gl_Position.w;
    vec2 p2 = screenSize * gl_in[2].gl_Position.xy / gl_in[2].gl_Position.w;
    vec2 v0 = p2 - p1;
    vec2 v1 = p2 - p0;
    vec2 v2 = p1 - p0;


    float area = abs(v1.x*v2.y - v1.y * v2.x);

    gl_Position = gl_in[0].gl_Position;
    distFromEdge = vec3(area/length(v0),0,0);

    gl_PrimitiveID = gl_PrimitiveIDIn;

    passData();

    EmitVertex();

    gl_Position = gl_in[1].gl_Position;
    distFromEdge = vec3(0,area/length(v1),0);

    gl_PrimitiveID = gl_PrimitiveIDIn;

    passData();

    EmitVertex();

    gl_Position = gl_in[2].gl_Position;
    distFromEdge = vec3(0,0,area/length(v2));

    gl_PrimitiveID = gl_PrimitiveIDIn;

    passData();

    EmitVertex();

    EndPrimitive();
}