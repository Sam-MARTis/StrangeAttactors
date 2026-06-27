#include "particles.hpp"
#include "aux.hpp"

Particle* initializeRandomParticles(unsigned int numParticles, const glm::vec2& xBounds, const glm::vec2& yBounds, const glm::vec2& zBounds){
    Particle* particles = new Particle[numParticles];
    for(unsigned int i=0; i<numParticles; i++){
        Particle& particle = particles[i];
        particle.position.x = randFloat(xBounds);
        particle.position.y = randFloat(yBounds);
        particle.position.z = randFloat(zBounds);
        particle.position.w = 1.0f;

    }
    return particles;
}


Particle* initializeRandomParticlesCentroids(unsigned int numParticles, const glm::vec3& centroid, const glm::vec3& halfScale){
    glm::vec2 xBounds(centroid.x - halfScale.x, centroid.x + halfScale.x);
    glm::vec2 yBounds(centroid.y - halfScale.y, centroid.y + halfScale.y);
    glm::vec2 zBounds(centroid.z - halfScale.z, centroid.z + halfScale.z);
    return initializeRandomParticles(numParticles, xBounds, yBounds, zBounds);

    // Particle* particles = new Particle[numParticles];
    // for(unsigned int i=0; i<numParticles; i++){
    //     Particle& particle = particles[i];
    //     particle.position.x = randFloat(xBounds);
    //     particle.position.y = randFloat(yBounds);
    //     particle.position.z = randFloat(zBounds);
    //     particle.position.w = 1.0f;
    //     particle.velocity.x = randFloat(vxBounds);
    //     particle.velocity.y = randFloat(vyBounds);
    //     particle.velocity.z = randFloat(vzBounds);
    //     particle.velocity.w = 0.0f;

    // }
    // return particles;
}