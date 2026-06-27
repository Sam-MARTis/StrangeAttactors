#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

layout (std140, binding=0) uniform CameraInformation{
   mat4 projection;
   mat4 view;
   vec3 cameraPos;
   vec3 cameraFront;
};

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;


uniform vec3 lightPos;
// uniform mat4 projection;
// uniform mat4 view;
uniform mat4 model;
uniform mat4 adjustedNormalModel;

void main(){
    FragPos = vec3(model* vec4(aPos, 1.0f));
    gl_Position = projection*view * vec4(FragPos, 1.0f);
    Normal = mat3(adjustedNormalModel) * aNormal; // mat3 cause we dont want normal translation. Not a point
    TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}