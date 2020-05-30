#version 330 core

layout (location = 0) in vec2 vertexPosition;
layout (location = 1) in vec2 uv;

uniform vec2 position;

void main()
{
    gl_Position = vec4(vertexPosition + position, 1.0, 1.0);
}