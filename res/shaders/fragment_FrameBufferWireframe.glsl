#version 330 core

in vec2 texCoords;

uniform sampler2D colorTex;
uniform sampler2D depthTex;

out vec4 fragColor;

void main() {

    vec4 depth = vec4(1,1,1,2) - texture(depthTex, texCoords);
    depth = vec4(step(0.000001f, depth.x),step(0.000001f, depth.x),step(0.000001f, depth.x),1);


    fragColor = texture(depthTex, texCoords);
}