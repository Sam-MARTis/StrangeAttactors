#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



struct Particle{
    glm::vec4 position;
};

Particle* initializeRandomParticles(unsigned int numParticles, const glm::vec2& xBounds, const glm::vec2& yBounds, const glm::vec2& zBounds);
Particle* initializeRandomParticlesCentroids(unsigned int numParticles, const glm::vec3& centroid, const glm::vec3& halfScale);