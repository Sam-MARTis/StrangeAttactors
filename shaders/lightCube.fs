#version 460 core
out vec4 FragColor;


uniform vec3 lightColour;

void main(){
    // float mix_value = gl_position
    
    FragColor = vec4(lightColour, 1.0f);
}