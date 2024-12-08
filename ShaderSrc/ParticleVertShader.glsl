#version 330 core
layout(location = 0) in vec3 vertexPos;
layout(location = 1) in vec3 InstancePos;
layout(location = 2) in float instanceLifetime;

uniform mat4 model;
uniform mat4 CamMatrix;
out float lifetime;

void main()
{
    vec4 worldPos = model * vec4(vertexPos,1.0) + vec4(InstancePos,1.0);
    gl_Position = CamMatrix * worldPos;
    lifetime = instanceLifetime;
}