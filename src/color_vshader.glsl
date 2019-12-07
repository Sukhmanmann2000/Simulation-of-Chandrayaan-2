#version 330

in vec4 vPosition;
in vec4 vColor;
out vec4 color;
uniform mat4 uModelViewMatrix;
uniform mat4 transMatrix;

void main (void) 
{
  gl_Position = uModelViewMatrix * transMatrix * vPosition;
  color = vColor;
}