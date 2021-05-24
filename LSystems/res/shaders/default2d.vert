#version 460 core

in vec2 aPosition;
in vec3 aColor;
in vec2 aTexCoords;

out vec2 texCoords;
out vec3 color;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
  
void main()
{
	texCoords = aTexCoords;
	color = aColor;
	gl_Position = uProjection * uView * uModel * vec4(aPosition, 0.0, 1.0);
}