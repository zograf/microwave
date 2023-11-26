#version 330 core

layout(location = 0) in vec2 inPos;
layout(location = 1) in vec4 inCol;
out vec4 channelCol;

uniform float offset;

void main() {
	gl_Position = vec4(inPos.x + offset, inPos.y, 0.0, 1.0);
	channelCol = inCol;
}
