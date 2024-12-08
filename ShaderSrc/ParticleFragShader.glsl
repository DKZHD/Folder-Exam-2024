#version 330 core
out vec4 FragColor;
in float lifetime;
void main()
{
    if(lifetime<=0)
    {
        discard;
    }

    FragColor = vec4(0.67, 0.67, 0.95, 0.37);
}