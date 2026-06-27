#version 460 core

// layout (location = 0) in vec4 aPos;

layout (std140, binding=0) uniform CameraInformation{
    mat4 projection;
    mat4 view;
    vec3 cameraPos;
    vec3 cameraFront;
};

struct Particle {
    vec4 position;
    vec4 velocity;
};

layout(std430, binding = 0) buffer ParticleBuffer {
    Particle particles[];
};

// out float particleSizeSquared;
// out float smoothStart;
uniform mat4 model;

void main(){
    vec4 aPos = particles[gl_VertexID].position;
    gl_Position = projection * view * model * aPos;
    float distanceToCamera = length(aPos.xyz - cameraPos);
    gl_PointSize = 1.0f / distanceToCamera * 3.0;
    // particleSizeSquared = 1.0f / (distanceToCamera * distanceToCamera) * 100.0;
    
    // gl_PointSize = 1.0f;
}
