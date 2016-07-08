#version 430

layout (location = 0) in vec2 vertices;
layout (location = 1) in vec2 uvs;

uniform float ScreenOffset;
uniform mat3 RotTransMat;

out vec2 interpolatedUVs;

void main()
{
    interpolatedUVs = uvs;

	vec3 translatedPos = RotTransMat * vec3(vertices, 1.0);
	
	gl_Position = vec4(translatedPos, 1.0);
    
	//gl_Position = vec4(vertices, 0.0, 1.0);
}