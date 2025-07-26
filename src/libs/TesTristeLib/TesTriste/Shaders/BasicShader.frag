#version 330 core
out vec4 FragColor;

in vec3 fragPos;

uniform vec3 meshColor;

void main()
{
    FragColor = vec4(meshColor, 1.0);
}