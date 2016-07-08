#version 430

layout (location = 0) in vec2 vertices;
layout (location = 1) in vec2 uvs;

uniform float ScreenOffset;

out vec2 interpolatedUVs;

void main()
{
    interpolatedUVs = uvs;
    gl_Position = vec4(vertices + ScreenOffset, 0.0, 1.0);
}