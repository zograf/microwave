#version 330 core

out vec4 outCol;
in vec4 inCol;

uniform float param;

void main() {
	outCol = vec4(param, inCol[1], inCol[2], inCol[3]);
}