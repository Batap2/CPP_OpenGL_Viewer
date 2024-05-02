#version 330 core

in vec2 texCoords;

uniform sampler2D colorTex;
uniform usampler2D depthTex;
uniform vec4 wireframeColor;
uniform float wireframeWidth;

out vec4 fragColor;



void main() {

    float offset = wireframeWidth / 1.7;

    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right
    );

    float kernel[9] = float[](
        -1, -1, -1,
        -1,  9, -1,
        -1, -1, -1
    );

    uint index[9];
    for(int i = 0; i < 9; i++)
    {

        index[i] = texture(depthTex, texCoords + offsets[i]).x;
    }
    vec3 col = vec3(0.0);

    bool sameIndex = true;

    for(int i = 0; i < 9; i++)
    {
        if (index[i] != index[4])
        {
            sameIndex = false;
            col = vec3(1.0f);
            break;
        }
    }

    vec4 finalCol;

    if(col == vec3(0.0f))
    {
        finalCol = texture(colorTex, texCoords);
    } else {
        finalCol = vec4(col, 1) * wireframeColor;
    }

    fragColor = finalCol ;

}