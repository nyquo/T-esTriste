#version 330 core
out vec4 FragColor;

in vec3 fragPos;

uniform vec3 meshColor;
uniform vec3 meshOrigin;
uniform float meshSize;

void main()
{
    vec3 origin = vec3(0.0, 0.0, 0.0);
    float dist = length(fragPos - meshOrigin);
    float dimming = clamp(1.0 - dist/meshSize, 0.0, 1.0);
    vec3 finalColor = meshColor * dimming;

    FragColor = vec4(finalColor, 1.0);
}