#version 330 core

in vec2 vTexCoord;
in vec3 fragPos;
in vec3 vNormal;

uniform vec3 viewPos;
uniform vec4 uColor;
uniform sampler2D uTexture;
uniform int uUseTexture;

out vec4 FragColor;

void main()
{
    vec3 lightPos = vec3(5.0, 5.0, 2.0);
    vec3 N = normalize(vNormal);
    vec3 L = normalize(lightPos - fragPos);
    vec3 V = normalize(viewPos - fragPos);
    vec3 R = reflect(-L, N);

    // lighting
    vec3 ambient = vec3(0.05);
    float diff_strength = 2.0;
    float diff = max(dot(N, L), 0.0);
    vec3 diffuse = diff_strength * diff * vec3(1.0);    
    // white light
    float specular_strength = 1.2;
    float spec = pow(max(dot(V, R), 0.0), 32);
    vec3 specular = specular_strength * spec * vec3(1.0);

    vec3 lighting = (ambient + diffuse + specular);
    vec3 result = lighting * uColor.rgb;

    if (uUseTexture != 0)
        FragColor = texture(uTexture, vTexCoord) * vec4(result, 1.0);
    else
        FragColor = vec4(result, 1.0);
}