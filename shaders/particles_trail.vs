#version 460 core



uniform uint frameCount;
uniform uint trailHistoryLength;

out float alpha;

layout (std140, binding=0) uniform CameraInformation{
    mat4 projection;
    mat4 view;
    vec3 cameraPos;
    vec3 cameraFront;
};

struct Particle {
    vec4 position;
    // vec4 velocity;
};

layout(std430, binding = 1) buffer TrailHistoryBuffer {
    vec4 trailHistory[];
};

uniform mat4 model;

void main(){
    uint baseIndex = frameCount % trailHistoryLength; //Remember - previous shader WRITES to this index. Use the next index as oldest.
    uint ringIndex = (baseIndex + 1 + gl_VertexID ) % trailHistoryLength;
    uint ssboIndex = gl_DrawID * trailHistoryLength + ringIndex;
    gl_Position = projection * view * model * vec4(trailHistory[ssboIndex].xyz, 1.0f);
    alpha = pow(float(gl_VertexID % trailHistoryLength) / float(trailHistoryLength - 1), 64);
    // alpha = 0.1f;
}