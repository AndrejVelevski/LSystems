#version 460 core

struct Light {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 vFragPosition;
in vec3 vColor;
in vec3 vNormal;

out vec4 FragColor;

uniform Light uLight;
uniform vec3 uCameraPosition;
uniform vec3 uColor;

uniform bool uLightingEnabled;

void main()
{
    if (uLightingEnabled)
    {
        // ambient
        vec3 ambient = uLight.ambient * vColor;
  	
        // diffuse 
        vec3 norm = normalize(vNormal);
        vec3 lightDir = normalize(-uLight.direction);  
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = uLight.diffuse * diff * vColor * uColor;  
    
        // specular
        vec3 viewDir = normalize(uCameraPosition - vFragPosition);
        vec3 reflectDir = reflect(-lightDir, norm);  
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
        vec3 specular = uLight.specular * spec;  
        
        vec3 result = ambient + diffuse + specular;
        FragColor = vec4(result, 1.0);
    }
    else
    {
        FragColor = vec4(vColor * uColor, 1.0);
    }
}