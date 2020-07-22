#version 330 core

in float zPos;

uniform float shadeMultiplier;

void main() {
        gl_FragColor = vec4(zPos * shadeMultiplier, zPos * shadeMultiplier, zPos * shadeMultiplier, 1.0f);
}
