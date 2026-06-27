#version 460 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D tex;

uniform float screenWidth;
uniform float screenHeight;

const int a = 3;
float dx = 1.0f/screenWidth;
float dy = 1.0f/screenHeight;

float kernel[9] = float[](
    0, 1, 0,
    1, -4, 1, 
    0, 1, 0
);

void main(){

    vec3 result = vec3(0.0f);
    // float mix_value = gl_position
    for(int i = -1; i < 2; i++) {
        for(int j=-1; j < 2; j++){
            result += texture(tex, vec2(TexCoord.x+i*dx, TexCoord.y+j*dy)).rgb * kernel[(i+1) + (3)*(j+1)];
        }
    }
    FragColor = vec4(result, 1.0f);

}