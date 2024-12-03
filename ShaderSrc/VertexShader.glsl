#version 330 core
layout(location = 0) in vec3 aPos; 
layout(location = 1) in vec3 aNormal; 
layout(location = 2) in vec3 aColor;

out vec3 FragPos;     
out vec3 Normal;
out vec3 objectColor;

uniform mat4 Model;
uniform mat4 CamMatrix;

void main()
{
    objectColor = aColor;
    Normal = normalize(aNormal);
    FragPos = vec3(Model * vec4(aPos, 1.0));
    gl_Position = CamMatrix * vec4(FragPos, 1.0);
}
