#version 460 core

in vec3 aPosition;
in vec3 aColor;

out vec3 vColor;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
  
void main()
{
	vColor = aColor;

	gl_Position = uProjection * uView * uModel * vec4(aPosition.x, aPosition.y, aPosition.z, 1.0);
}