#version 460 core
out vec4 FragColor;


uniform vec3 lightColour;

// in float particleSizeSquared;
void main(){
    vec2 coord = gl_PointCoord * 2.0 - 1.0;
    float dist = dot(coord, coord); 
    if (dist > 1.0) discard;
    FragColor = vec4(lightColour, 1.0f);
    // FragColor = vec4(vec3(0.1f), 1.0f);
}