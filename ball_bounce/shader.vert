#version 330 core

layout (location = 0) in vec2 Pos; 
layout (location =1) in float mass;

layout (location =2) in vec2 Shift;
layout (location =3) in vec4 Color;
layout (location =4) in float redius;
out vec4 BodyColor;
uniform float aspectRatio;
uniform vec2 scroll;
uniform float zoom;



void main() {

vec2 finalPos = (Pos * redius) ;
finalPos.y *= aspectRatio;
finalPos += Shift;
finalPos.x -= scroll.x;
finalPos.y += scroll.y;


gl_Position = vec4(finalPos*zoom, 0.0, 1.0);
if(Color == vec4(0.0,0.0,0.0,0.0)){
float factor = pow(1-redius,10);
BodyColor = vec4(1-factor,1.0,factor,1.0);

}else{
BodyColor = Color;

}

}