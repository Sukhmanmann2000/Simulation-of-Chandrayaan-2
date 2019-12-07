#version 330

in vec4 vPosition;
in vec2 texCoord;
out vec4 ParticleColor;
varying vec2 tex;
uniform vec4 color;
uniform mat4 particlemodelViewMatrix;
uniform mat4 particlebasetransMatrix;
uniform mat4 particleinitMatrix;

void main (void) 
{
  gl_Position = particlemodelViewMatrix * particlebasetransMatrix * particleinitMatrix * vPosition;
  ParticleColor = color;
  tex = texCoord;
}
