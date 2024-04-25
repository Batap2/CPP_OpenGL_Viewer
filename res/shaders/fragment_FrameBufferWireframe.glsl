#version 330 core

in vec2 texCoords;

uniform sampler2D colorTex;
uniform sampler2D depthTex;
uniform vec4 wireframeColor;
uniform float wireframeWidth;

out vec4 fragColor;


vec4 processDepth(vec4 depth){
    vec4 d = vec4(1,1,1,2) - depth;
    return vec4(step(0.000001f, d.x),step(0.000001f, d.x),step(0.000001f, d.x),1);
}

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

    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = processDepth(texture(depthTex, texCoords + offsets[i])).xyz;
    }
    vec3 col = vec3(0.0);

    bool black = false;
    bool white = false;

    for(int i = 0; i < 9; i++)
    {
        if (sampleTex[i] == vec3(1.0f))
            white = true;
        if (sampleTex[i] == vec3(0.0f))
            black = true;
    }

    if(black && white)
            col = vec3(1.0f);

    vec4 finalCol;

    if(col == vec3(0.0f))
    {
        finalCol = texture(colorTex, texCoords);
    } else {
        finalCol = vec4(col, 1) * wireframeColor;
    }

    //fragColor = vec4(col,1) * wireframeColor + texture(colorTex, texCoords) ;
    fragColor = finalCol;

    //fragColor = vec4(col, 1);
}