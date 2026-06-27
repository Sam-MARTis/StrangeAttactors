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
    -1, 1, -1,
    1, 2, 1, 
    -1, 1, -1
);

void main(){
    // float scaling_factor = 1.0f;
    vec3 contrib = vec3(0.0f);
    // // float mix_value = gl_position
    for(int i = -1; i < 2; i++) {
        for(int j=-1; j < 2; j++){
            vec3 value = texture(tex, vec2(TexCoord.x+i*dx, TexCoord.y+j*dy)).rgb;
            contrib += 0.33333f*(value.x + value.y + value.z) * kernel[(i+1) + (3)*(j+1)];
        }
    }
    // contrib /= scaling_factor;
    contrib *= 0.25f;
    


    // FragColor = vec4(result, 1.0f);
    FragColor = texture(tex, TexCoord) + vec4(contrib, 1.0f);

}