#version 330 core

in vec2 uv;

uniform vec4 wireframeColor;

out vec4 fragColor;

void main (void)
{
        float pattern = step(0,cos(uv.x*10));

        fragColor = wireframeColor;
}