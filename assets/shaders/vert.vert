#version 330 core

layout (location = 0) in vec3 vertexPosition;

out float zPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float zScale;

void main()
{
    gl_Position = projection * view * model * vec4(vertexPosition.xy, vertexPosition.z * zScale, 1.0);
    zPos = vertexPosition.z;
}