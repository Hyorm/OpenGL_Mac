#version 410

in vec4 vPosition;
uniform mat4 T;

void main() {
	gl_Position = T * vPosition; 
}
