#version 330 core

out vec4 FragColor;
in vec2 textureCoord;

uniform sampler2D ship;

void main()
{
    FragColor = texture(ship, textureCoord);
}