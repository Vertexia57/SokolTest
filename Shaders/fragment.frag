#version 330

uniform sampler2D samplerChannel0;

in vec2 texUV;
in vec2 screenXY;
in vec4 iColor;
layout(location = 0) out vec4 fragColor;

void main()
{
    fragColor = texture(samplerChannel0, texUV.xy) * iColor;
}