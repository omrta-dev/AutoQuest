#version 330 core

in vec2 inputUv;

uniform sampler2D diffuseTexture;

void main() {
    gl_FragColor = texture(diffuseTexture, inputUv);
}
