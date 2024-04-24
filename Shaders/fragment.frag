#version 330

uniform vec4 uniforms[2];
uniform sampler2D samplerChannel0;
uniform sampler2D samplerChannel1;

in vec2 texUV;
in vec4 iColor;
layout(location = 0) out vec4 fragColor;

void main()
{
    fragColor = texture(samplerChannel0, texUV) * iColor;
}