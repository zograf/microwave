#version 330 core

layout(location = 0) in vec2 inPos;
layout(location = 1) in vec4 inCol;
out vec4 channelCol;

void main() {
	gl_Position = vec4(inPos.x, inPos.y, 0.0, 1.0);
	channelCol = inCol;
}