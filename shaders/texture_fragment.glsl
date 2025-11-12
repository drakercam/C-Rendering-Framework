#version 330 core

in vec2 vTexCoord;

uniform sampler2D uTexture;
uniform bool uUseTexture;

out vec4 FragColor;

void main()
{
    FragColor = texture(uTexture, vTexCoord);
}