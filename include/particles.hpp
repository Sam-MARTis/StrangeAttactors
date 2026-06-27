#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



struct Particle{
    glm::vec4 position;
    glm::vec4 velocity;
};

Particle* initializeRandomParticles(unsigned int numParticles, const glm::vec2& xBounds, const glm::vec2& yBounds, const glm::vec2& zBounds, const glm::vec2& vxBounds, const glm::vec2& vyBounds, const glm::vec2& vzBounds);