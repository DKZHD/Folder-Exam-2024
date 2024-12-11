#version 330 core
out vec4 FragColor;
in float lifetime;
void main()
{
    if(lifetime<=0)
    {
        discard;
    }

    FragColor = vec4(0.61, 0.61, 1.0, 0.45);
}