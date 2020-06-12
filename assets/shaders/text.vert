#version 330 core
layout (location = 0) in vec4 vertexPosition;
varying vec2 texpos;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * vec4(vertexPosition.xy, 1.0, 1.0);
    texpos = vertexPosition.zw;
}