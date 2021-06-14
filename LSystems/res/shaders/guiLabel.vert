#version 330 core

in vec2 aPosition;
in vec2 aTexCoords;

out vec2 texCoords;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
  
void main()
{
	texCoords = aTexCoords;
	gl_Position = uProjection * uView * uModel * vec4(aPosition, 0.0, 1.0);
}