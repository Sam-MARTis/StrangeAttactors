#pragma once 

#include <glm/glm.hpp>

constexpr unsigned int SCREEN_WIDTH = 1000;
constexpr unsigned int SCREEN_HEIGHT = 1000;
constexpr float PI = 3.14159265358979323846f;
constexpr float SCROLL_SENSITIVITY = 0.1f;
constexpr float ROTATION_SENSITIVITY = 0.0015f;

constexpr float MAX_DS = 10.0f;
constexpr float KEYBOARD_MOVE_SPEED = 30.0f;

inline float clamp(float x, float min, float max){
    return x<min?min: x>max?max:x;
}

constexpr unsigned int MAX_PARTICLES = 128*256;
constexpr float DT_FACTOR  = .2f;

constexpr int TRAIL_HISTORY_LENGTH = 2;


constexpr glm::vec3 lorenz_centroid = glm::vec3(0.0f, 0.0f, 25.0f);
constexpr glm::vec3 lorenz_scaling = glm::vec3(0.6f, 0.6f, 0.6f);
constexpr glm::vec3 lorenz_half_scale = glm::vec3(10.0f, 30.0f, 20.0f);

constexpr glm::vec3 halvorsen_centroid = glm::vec3(-8.0f, -5.0f, -5.0f);
constexpr glm::vec3 halvorsen_scaling = glm::vec3(1.0f, 1.0f, 1.0f);
constexpr glm::vec3 halvorsen_half_scale = glm::vec3(10.0f, 8.0f, 10.0f);



#ifndef SHADER_DIR
#define SHADER_DIR "shaders/"
#endif

#ifndef TEXTURE_DIR
#define TEXTURE_DIR "resources/textures/"
#endif

#ifndef MODEL_DIR
#define MODEL_DIR "resources/models/"
#endif

const std::string vertexShaderFilePath = std::string(SHADER_DIR) + "shader.vs";
const std::string fragmentShaderFilePath = std::string(SHADER_DIR) + "shader.fs";
const std::string lightCubeFilePath = std::string(SHADER_DIR) +"lightCube";
const std::string backpackFilePath = std::string(SHADER_DIR) + "backpack";
const std::string fbQuadFilePath = std::string(SHADER_DIR) + "framebuffer_particles";
const std::string particlesFilePath = std::string(SHADER_DIR) + "particles";
const std::string particlesTrailFilePath = std::string(SHADER_DIR) + "particles_trail";