#version 330 core

layout (location = 0) in vec2 Pos; 
uniform vec2 scroll;
out vec4 BodyColor;

uniform float zoom;


void main() {
vec2 finalPos = Pos;
finalPos.x -= scroll.x;
finalPos.y += scroll.y;

gl_Position = vec4(finalPos*zoom, 0.0, 1.0);
BodyColor = vec4(1.0,1.0,1.0,1.0);

}