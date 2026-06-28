#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include"display-objects.hpp"
#include "constants.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "aux.hpp"
#include "display-objects.hpp"
#include "particles.hpp"

// class Camera{
//     public: 
//     glm::vec3 pos;
//     glm::vec3 front;
//     glm::vec3 up;
//     Camera(glm::vec3 cameraPos, glm::vec3 cameraFront, glm::vec3 cameraUp, float fov_degrees, float aspect_ratio, float min_view_distance, float max_view_distance);
//     void move(glm::vec3 cameraMovementVector);
//     glm::mat4 getViewMatrix();
//     glm::mat4 getProjectionMatrix();
//     // void turn(glm::vec3 cameraTurnVec; float anglesInDegrees);
//     private:
//     glm::vec3 right;
//     glm::mat4 viewMat;
//     glm::mat4 projMat;

//     float fov_deg;
//     float aspect_ratio;
//     float min_view_distance;
//     float max_view_distance;
// };

Camera::Camera(glm::vec3 cameraPos, glm::vec3 cameraFront, glm::vec3 cameraUp, float cam_fov_degrees, float cam_aspect_ratio, float cam_min_view_distance, float cam_max_view_distance){
    pos=  cameraPos;
    front = cameraFront;
    up = cameraUp;
    right = glm::cross(front, up);
    viewMat = glm::lookAt(pos, pos+front, up);
    fov_deg = cam_fov_degrees;
    aspect_ratio = cam_aspect_ratio;
    min_view_distance = cam_min_view_distance;
    max_view_distance = cam_max_view_distance;
    projMat = glm::perspective(glm::radians(fov_deg), aspect_ratio, min_view_distance, max_view_distance);
}
void Camera::recalculateProjectionMatrix(){
    projMat = glm::perspective(glm::radians(fov_deg), aspect_ratio, min_view_distance, max_view_distance);
}
void Camera::recalculateViewMatrix(){
    viewMat = glm::lookAt(pos, pos+front, up);
}
glm::mat4 Camera::getViewMatrix(){
    return viewMat;
}
glm::mat4 Camera::getProjectionMatrix(){
    return projMat;
}
void Camera::handleMouseScroll(float yOffset){
    pos += up*SCROLL_SENSITIVITY*yOffset;
    viewMat = glm::lookAt(pos, pos+front, up);
}

void Camera::handleMouseMove(float xOffset, float yOffset){
    if(xOffset*xOffset + yOffset*yOffset <= 1e-3) return;
    glm::vec3 offsetVec = glm::normalize(right*xOffset + up*yOffset);
    glm::vec3 rotationVec = glm::cross(front, offsetVec);
    float rotationValue = sqrtf(xOffset*xOffset + yOffset*yOffset) * ROTATION_SENSITIVITY;
    glm::mat4 rotMat = glm::mat4(1.0f);
    rotMat = glm::rotate(rotMat, rotationValue, rotationVec);
    up = glm::vec3(rotMat* glm::vec4(up, 0.0f));
    right = glm::vec3(rotMat* glm::vec4(right, 0.0f));
    front = glm::vec3(rotMat* glm::vec4(front, 0.0f));
    
    // right *= rotMat;
    // front *= rotMat;
    viewMat = glm::lookAt(pos, pos+front, up);
}


void Camera::move(glm::vec3 cameraMovementVector){
    pos += cameraMovementVector;
    viewMat = glm::lookAt(pos, pos+front, up);
}

// void renderImGuiMenu(ImGUIData_In& data_in){
    
    // ImGui::Text("FPS: %.1f", 1.0f / *(data_in.deltaT));
    // ImGui::Text("Frame Count: %d", *(data_in.frameCount));
    // ImGui::Text("Camera Position: (%.2f, %.2f, %.2f)", data_in.cam->pos.x, data_in.cam->pos.y, data_in.cam->pos.z);


    // // add your controls here

void renderImGuiMenu(ImGUIData_In& data_in) {
    ImGui::Text("FPS: %.1f", 1.0f / *(data_in.deltaT));
    ImGui::Text("Frame Count: %d", *(data_in.frameCount));
    ImGui::Text("Camera Position: (%.2f, %.2f, %.2f)",
        data_in.cam->pos.x, data_in.cam->pos.y, data_in.cam->pos.z);

    ImGui::Separator();

    if (ImGui::Button("Lorenz")) {
        *(data_in.simType) = 0; 
        
        // data_in.cam->pos = lorenz_centroid + glm::vec3(0.0f, 0.0f, 105.0f);
        // data_in.cam->pos = glm::vec3(0.0f, 0.0f, 135.0f);

        
        Particle* particles = initializeRandomParticlesCentroids(
            MAX_PARTICLES, lorenz_centroid, lorenz_half_scale);

        
        glNamedBufferSubData(
            *(data_in.SSBO),
            0,
            sizeof(Particle) * MAX_PARTICLES,
            particles
        );

        delete[] particles;  
    }

    ImGui::SameLine();  

    if (ImGui::Button("Halvorsen")) {
        *(data_in.simType) = 1; 
        data_in.cam->pos = halvorsen_centroid + glm::vec3(0.0f, 0.0f, 35.0f);
        data_in.cam->pos = glm::vec3(-10.0f, -5.0f, 35.0f);

        Particle* particles = initializeRandomParticlesCentroids(
            MAX_PARTICLES, halvorsen_centroid, halvorsen_half_scale);

        glNamedBufferSubData(
            *(data_in.SSBO),
            0,
            sizeof(Particle) * MAX_PARTICLES,
            particles
        );

        delete[] particles;
    }
    // data_in.cam->recalculateViewMatrix();
}
    
