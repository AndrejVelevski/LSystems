#version 460 core

in vec3 color;
in vec2 texCoords;

out vec4 FragColor;

uniform vec3 uColor;
uniform sampler2D uTexture;

void main()
{
	//vec4 color = texture(uTexture, texCoords);

	vec4 sampled = vec4(1.0, 1.0, 1.0, texture(uTexture, texCoords).r);

	FragColor = vec4(color, 1.0) * sampled;

	//FragColor = texture(uTexture, texCoords) * vec4(color, 1.0);

	//discard;
}