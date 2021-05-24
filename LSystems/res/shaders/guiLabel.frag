#version 460 core

in vec2 texCoords;

out vec4 FragColor;

uniform vec3 uColor;
uniform sampler2D uTexture;

void main()
{
	FragColor = vec4(uColor, 1.0) * texture(uTexture, texCoords);
	if (FragColor.a < 0.01)
		discard;
}