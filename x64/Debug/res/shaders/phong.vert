#version 460 core

in vec3 aPosition;
in vec3 aNormal;
in vec3 aColor;

out vec3 vFragPosition;
out vec3 vColor;
out vec3 vNormal;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
  
void main()
{
	vFragPosition = vec3(uModel * vec4(aPosition, 1.0));
	vColor = aColor;
	vNormal = mat3(transpose(inverse(uModel))) * aNormal;  

	gl_Position = uProjection * uView * uModel * vec4(aPosition.x, aPosition.y, aPosition.z, 1.0);
}