#version 330 core

layout (lines) in;
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

    vec4 p0 = gl_in[0].gl_Position;
    vec4 p1 = gl_in[1].gl_Position;

    float t0 = p0.z + p0.w;
    float t1 = p1.z + p1.w;
    if(t0 < 0.0)
    {
        if(t1 < 0.0)
        {
            return;
        }
        p0 = mix(p0, p1, (0 - t0) / (t1 - t0));
    }
    if(t1 < 0.0)
    {
        p1 = mix(p0, p1, (0 - t0) / (t1 - t0));
    }

    vec2 startInNDC     = p0.xy / p0.w;       //  clip to NDC: homogenize and drop z
    vec2 endInNDC       = p1.xy / p1.w;
    // --------- 1

    vec2 line = endInNDC - startInNDC;
    vec2 n = normalize(vec2(-line.y, line.x)) * wireframeWidth;
    vec2 offset = normalize(line) * wireframeWidth;

    gl_Position = vec4(startInNDC + n - offset,0,1);
    EmitVertex();

    gl_Position = vec4(startInNDC - n - offset,0,1);
    EmitVertex();

    gl_Position = vec4(endInNDC + n + offset,0,1);
    EmitVertex();

    gl_Position = vec4(endInNDC - n + offset,0,1);
    EmitVertex();


//    gl_Position = vec4(p1 + n_p0p1*wireframeWidth, 0,1);
//    EmitVertex();
//
//    gl_Position = vec4(p1 - n_p0p1*wireframeWidth, 0,1);
//    EmitVertex();





    // --------- 2

//    gl_Position = vec4(p1 + n_p1p2*wireframeWidth, 0,1);
//    EmitVertex();
//
//    gl_Position = vec4(p1 - n_p1p2*wireframeWidth, 0,1);
//    EmitVertex();
//
//    gl_Position = vec4(p2 + n_p2p0*wireframeWidth, 0,1);
//    EmitVertex();
//
//    gl_Position = vec4(p2 - n_p2p0*wireframeWidth, 0,1);
//    EmitVertex();

    EndPrimitive();
}