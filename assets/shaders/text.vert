#version 330 core

layout (location = 0) in vec2 vertexPosition;
layout (location = 1) in vec2 uv;

uniform vec2 position;
uniform mat4 view;
uniform mat4 projection;
uniform vec2 scale;

void main()
{
    gl_Position = projection * view * vec4((vertexPosition + scale) + position, 1.0, 1.0);
}