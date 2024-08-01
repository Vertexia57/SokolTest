#version 330

uniform sampler2D samplerChannel0;

uniform highp vec4 uniforms[1];

in vec2 texUV;
in vec2 screenXY;
in vec4 iColor;
layout(location = 0) out vec4 fragColor;

float hash(float n) { return fract(sin(n) * 1e4); }
float hash(vec2 p) { return fract(1e4 * sin(17.0 * p.x + p.y * 0.1) * (0.1 + abs(sin(p.y * 13.0 + p.x)))); }

float noise(vec2 x) {
	vec2 i = floor(x);
	vec2 f = fract(x);

	// Four corners in 2D of a tile
	float a = hash(i);
	float b = hash(i + vec2(1.0, 0.0));
	float c = hash(i + vec2(0.0, 1.0));
	float d = hash(i + vec2(1.0, 1.0));

	// Simple 2D lerp using smoothstep envelope between the values.
	// return vec3(mix(mix(a, b, smoothstep(0.0, 1.0, f.x)),
	//			mix(c, d, smoothstep(0.0, 1.0, f.x)),
	//			smoothstep(0.0, 1.0, f.y)));

	// Same code, with the clamps in smoothstep and common subexpressions
	// optimized away.
	vec2 u = f * f * (3.0 - 2.0 * f);
	return mix(a, b, u.x) + (c - a) * u.y * (1.0 - u.x) + (d - b) * u.x * u.y;
}

void main()
{
    vec2 res = vec2(uniforms[0].x, uniforms[0].y);
    vec2 uv = floor(texUV / 2.0f * res) * 2.0f / res;

    vec3 outCol = step(0.0f, uv.y) * (vec3(0.3, 0.1, 0.4) * (uv.y) / 1.0f / 2.0f * min(noise(vec2(uv.x / 80.0f, texUV.y / 80.0f)) + (uv.y) / 1.0f * 1.2f, 1.0f));
    
    fragColor = mix(vec4(outCol, 1.0f), texture(samplerChannel0, uv.xy) * iColor, texture(samplerChannel0, uv.xy).a);
}