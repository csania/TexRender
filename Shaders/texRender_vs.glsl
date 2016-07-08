#version 120

attribute vec2 vertices;
attribute vec2 uvs;

varying vec2 interpolatedUVs;

void main()
{
    interpolatedUVs = uvs;
    gl_Position = vec4(vertices, 0.0, 1.0);
}