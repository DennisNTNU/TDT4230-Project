#version 450 core

in highp vec4 fragmentColor;

out highp vec4 color;

void main(){
	color = fragmentColor;
}