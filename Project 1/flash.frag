#version 330 core

out vec4 outCol;
in vec4 inCol;

uniform vec4 param;

void main() {
	outCol = param;
}