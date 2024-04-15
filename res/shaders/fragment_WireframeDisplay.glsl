#version 330 core

noperspective in vec3 distFromEdge;

out vec4 fragColor;

void Barycentric(vec3 p, vec3 a, vec3 b, vec3 c, inout float u, inout float v, inout float w)
{
        vec3 v0 = b - a, v1 = c - a, v2 = p - a;
        float d00 = dot(v0, v0);
        float d01 = dot(v0, v1);
        float d11 = dot(v1, v1);
        float d20 = dot(v2, v0);
        float d21 = dot(v2, v1);
        float denom = d00 * d11 - d01 * d01;
        v = (d11 * d20 - d01 * d21) / denom;
        w = (d00 * d21 - d01 * d20) / denom;
        u = 1.0f - v - w;
}

void main (void)
{
        float nearD = min(min(distFromEdge[0],distFromEdge[1]),distFromEdge[2]);
        float edgeIntensity = exp(-1.0*nearD*nearD);

        fragColor = vec4(edgeIntensity,0,0, edgeIntensity);
}