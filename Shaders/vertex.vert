#version 330

layout(location = 0) in vec4 coord;
out vec2 texUV;
out vec2 screenXY;
out vec4 iColor;
layout(location = 1) in vec4 color;

void main()
{
    gl_Position = vec4(coord.xy, 0.0, 1.0);
    texUV = coord.zw;
    iColor = color;
    screenXY = coord.xy;
}

