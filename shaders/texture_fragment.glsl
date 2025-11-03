#version 330 core

in vec2 vTexCoord;

uniform vec4 uColor;
uniform sampler2D uTexture;
uniform bool uUseTexture;

out vec4 FragColor;

void main()
{
    if (uUseTexture)
        FragColor = texture(uTexture, vTexCoord) * uColor;
    else
        FragColor = uColor;
}