#version 410 core

in vec3 textureCoordinates;
out vec4 color;

uniform samplerCube night;

void main()
{
    color = texture(night, textureCoordinates);
}
