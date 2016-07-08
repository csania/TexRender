#version 120

varying vec2 interpolatedUVs;
uniform sampler2D TexSampler;
void main()
{
    gl_FragColor = texture2D(TexSampler, interpolatedUVs);
}