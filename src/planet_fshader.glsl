#version 330

out vec4 frag_color;

uniform sampler2D texture;
varying vec2 tex;
void main () 
{
  frag_color = texture2D(texture, tex);		
}