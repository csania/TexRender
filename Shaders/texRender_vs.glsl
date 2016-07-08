#version 430

attribute vec2 vertices;
attribute vec2 uvs;

out vec2 interpolatedUVs;

void main()
{
    interpolatedUVs = uvs;
    gl_Position = vec4(vertices, 0.0, 1.0);
}