#version 330 core
in vec3 FragPos;       
in vec3 Normal; 
in vec3 objectColor; 

out vec4 FragColor;
uniform vec3 viewPos;

void main()
{
    vec3 edge1 = dFdx(FragPos);
    vec3 edge2 = dFdy(FragPos);
    vec3 aNormal = normalize(cross(edge1,edge2));
	vec3 lightColor = vec3(1.0, 1.0, 1.0);

    vec3 NormalizedNormal = normalize(aNormal);

    // Ambient lighting
    float ambientStrength = 0.6;
    vec3 ambient = ambientStrength * lightColor;

	vec3 lightPos = vec3(-300,400,-100);
    // Diffuse lighting
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(NormalizedNormal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    //Specular lighting
    vec3 viewDir = normalize(viewPos-FragPos);
    vec3 reflectDir = reflect(-lightDir,NormalizedNormal);
    float VdR = pow(max(dot(viewDir, reflectDir), 0.0), 64.0);

    vec3 specular = VdR * lightColor * 0.5;

    // Combine results
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}