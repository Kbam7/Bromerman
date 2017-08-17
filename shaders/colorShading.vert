#version 130

in vec2 vertexPosition;

void main() {
    gl_Postition.xy = vertexPosition;
    gl_Postiion.z = 0.0;
    gl_Position.w = 1.0;
}