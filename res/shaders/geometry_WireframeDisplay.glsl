#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 12) out;

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
uniform float wireframeWidth;

noperspective out vec3 distFromEdge;

void main() {

    vec2 p0 = screenSize * gl_in[0].gl_Position.xy / gl_in[0].gl_Position.w;
    vec2 p1 = screenSize * gl_in[1].gl_Position.xy / gl_in[1].gl_Position.w;
    vec2 p2 = screenSize * gl_in[2].gl_Position.xy / gl_in[2].gl_Position.w;
    vec2 v0 = p2 - p1;
    vec2 v1 = p2 - p0;
    vec2 v2 = p1 - p0;

    vec2 p0p1 = normalize(p1 - p0);
    vec2 p1p2 = normalize(p2 - p1);
    vec2 p2p0 = normalize(p0 - p2);

    vec2 n_p0p1 = vec2(-p0p1.y, p0p1.x);
    vec2 n_p1p2 = vec2(-p1p2.y, p1p2.x);
    vec2 n_p2p0 = vec2(-p2p0.y,-p2p0.x);

    float area = abs(v1.x*v2.y - v1.y * v2.x);


    // --------- 1


    gl_Position = gl_in[0].gl_Position + vec4(n_p0p1/20,0,0);
    EmitVertex();

    gl_Position = gl_in[0].gl_Position - vec4(n_p0p1/20,0,0);;
    EmitVertex();

    gl_Position = gl_in[1].gl_Position + vec4(n_p0p1/20,0,0);
    EmitVertex();

    gl_Position = gl_in[1].gl_Position - vec4(n_p0p1/20,0,0);;
    EmitVertex();

    // --------- 2

    EndPrimitive();
}