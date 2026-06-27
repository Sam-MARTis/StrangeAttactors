#version 460 core

in float alpha;

out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0, 0.0, 0.5, alpha);
}

