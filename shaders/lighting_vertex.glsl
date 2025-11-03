#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

out vec2 vTexCoord;
out vec3 vNormal;
out vec3 fragPos;

void main()
{
    vNormal = mat3(transpose(inverse(uModel))) * aNormal;
    fragPos = vec3(uModel * vec4(aPos, 1.0));

    vTexCoord = aTexCoord;
    gl_Position = uProjection * uView * vec4(fragPos, 1.0);
}