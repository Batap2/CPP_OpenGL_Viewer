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

noperspective out vec3 distFromEdge;

void main() {

        vec2 p0 = screenSize * gl_in[0].gl_Position.xy / gl_in[0].gl_Position.w;
        vec2 p1 = screenSize * gl_in[1].gl_Position.xy / gl_in[1].gl_Position.w;
        vec2 p2 = screenSize * gl_in[2].gl_Position.xy / gl_in[2].gl_Position.w;
        vec2 v0 = p2 - p1;
        vec2 v1 = p2 - p0;
        vec2 v2 = p1 - p0;

        vec2 triCenter = (p0+p1+p2)/3;

        float area = abs(v1.x*v2.y - v1.y * v2.x);

        vec2 centerToPos;

        centerToPos = gl_in[0].gl_Position.xy - triCenter;

        gl_Position = gl_in[0].gl_Position - vec4(0,0,0.001,0);
        distFromEdge = vec3(area/length(v0),0,0);

        gl_PrimitiveID = gl_PrimitiveIDIn;

        EmitVertex();

        centerToPos = gl_in[1].gl_Position.xy - triCenter;

        gl_Position = gl_in[1].gl_Position - vec4(0,0,0.001,0);
        distFromEdge = vec3(0,area/length(v1),0);

        gl_PrimitiveID = gl_PrimitiveIDIn;

        EmitVertex();

        centerToPos = gl_in[2].gl_Position.xy - triCenter;

        gl_Position = gl_in[2].gl_Position - vec4(0,0,0.001,0);
        distFromEdge = vec3(0,0,area/length(v2));

        gl_PrimitiveID = gl_PrimitiveIDIn;

        EmitVertex();

    EndPrimitive();
}