#version 460 core

in float alpha;

out vec4 FragColor;

void main()
{
    
    // FragColor = vec4(0.7f-alpha*0.7f, 0.0, 0.8f*alpha, alpha);
    FragColor = vec4(0.7f+alpha*0.3f, 0.0, 0.8f*alpha, alpha);
}

