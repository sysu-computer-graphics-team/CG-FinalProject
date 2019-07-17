#version 330 core
in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D snowTexture;

void main()
{
    // FragColor = ParticleColor;
    vec4 texColor = texture(snowTexture, TexCoords);
    if(texColor.a < 0.1)
        discard;

    FragColor = texColor;
}