#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include "aux.hpp"
#include "constants.hpp"
#include "stb_image.h"
#include "display-objects.hpp"
#include "processing-objects.hpp"
#include "shaders.hpp"
#include "particles.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


#ifndef SHADER_DIR
#define SHADER_DIR "shaders/"
#endif

#ifndef TEXTURE_DIR
#define TEXTURE_DIR "resources/textures/"
#endif

#ifndef MODEL_DIR
#define MODEL_DIR "resources/models/"
#endif
int main()
{
    

    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 objectPos = glm::vec3(0.0f, 0.0f, -1.0f);
    float fov = 65.0f;
    float AR = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;
    float min_view_dist = 0.1f;
    float max_view_dist = 250.f;
    
    
    Camera cam(cameraPos, cameraFront, cameraUp, fov, AR, min_view_dist, max_view_dist);
    GLFWwindow *window = setupWindow(cam, SCREEN_WIDTH, SCREEN_HEIGHT);
    // Add this right after gladLoadGLLoader
std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
// Right after gladLoadGLLoader
if(!glCreateBuffers)
    std::cout << "glCreateBuffers not loaded" << std::endl;
if(!glNamedBufferStorage)
    std::cout << "glNamedBufferStorage not loaded" << std::endl;
    glEnable(GL_DEPTH_TEST);
    // std::cout << std::string(SHADER_DIR) + "shader.vs" << std::endl;
    const std::string vertexShaderFilePath = std::string(SHADER_DIR) + "shader.vs";
    const std::string fragmentShaderFilePath = std::string(SHADER_DIR) + "shader.fs";
    const std::string lightCubeFilePath = std::string(SHADER_DIR) +"lightCube";
    const std::string backpackFilePath = std::string(SHADER_DIR) + "backpack";
    const std::string fbQuadFilePath = std::string(SHADER_DIR) + "framebuffer_particles";
    const std::string particlesFilePath = std::string(SHADER_DIR) + "particles";
    const std::string particlesTrailFilePath = std::string(SHADER_DIR) + "particles_trail";


    // Shader myShaders = Shader(st, fragmentShaderFilePath);
    // GraphicsShaders lightCubeShader = GraphicsShaders(lightCubeFilePath+".vs", lightCubeFilePath+".fs");
    // GraphicsShaders backpackShader = GraphicsShaders(backpackFilePath+".vs", backpackFilePath+".fs");
    GraphicsShaders fbQuadShader = GraphicsShaders(fbQuadFilePath+".vs", fbQuadFilePath+".fs");
    GraphicsShaders particlesShader = GraphicsShaders(particlesFilePath+".vs", particlesFilePath+".fs");
    GraphicsShaders particlesTrailShader = GraphicsShaders(particlesTrailFilePath+".vs", particlesTrailFilePath+".fs");
    ComputeShaders particlesComputeShader = ComputeShaders(particlesFilePath+".comp");
    ComputeShaders particlesTrailComputeShader = ComputeShaders(particlesTrailFilePath+".comp");

    

    glm::vec3 lightPos(1.0f, 0.8f, 1.5f);
    glm::vec3 objectColour(0.7f, 0.2f, 0.2f);
    glm::vec3 lightColour(0.9f, 0.9f, 0.9f);

    // glBindBuffer(GL_ARRAY_BUFFER, 0); // This is for unbinding
    // glBindVertexArray(0);             // Unbinding vao
    // const float dt = 1.0f / 60.0f;

    float prevFrameTime = static_cast<float>(glfwGetTime());
    std::string backPackFilePath = MODEL_DIR + std::string("backpack/backpack.obj");
    // Model backPack(backPackFilePath);






    float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
        // positions   // texCoords
        -1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
         1.0f, -1.0f, 0.0f,  1.0f, 0.0f,

        -1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
         1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
         1.0f,  1.0f, 0.0f,  1.0f, 1.0f
    };

    unsigned int quadVBO, quadVAO;

    glCreateVertexArrays(1, &quadVAO);
    glCreateBuffers(1, &quadVBO);

    glNamedBufferStorage(quadVBO, sizeof(quadVertices), &quadVertices, 0);
    
    glVertexArrayVertexBuffer(quadVAO, 0, quadVBO, 0, 5*sizeof(float));

    glVertexArrayAttribFormat(quadVAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribFormat(quadVAO, 1, 2, GL_FLOAT, GL_FALSE, 3*sizeof(float));

    glVertexArrayAttribBinding(quadVAO, 0, 0);
    glVertexArrayAttribBinding(quadVAO, 1, 0);
    


    glEnableVertexArrayAttrib(quadVAO, 0);
    glEnableVertexArrayAttrib(quadVAO, 1);
    glEnableVertexArrayAttrib(quadVAO, 2);

    
    int width = SCREEN_WIDTH;
    int height = SCREEN_HEIGHT;
    int nChannels = 3;
    unsigned int quadTexture = readTextureFromFile("", width, height, nChannels);
    if(width!=SCREEN_WIDTH & height != SCREEN_HEIGHT) std::cout<<"ERROR::TEXTURE_READ_ANOMOLY::width and height changed"<<std::endl;
    while(glGetError() != GL_NO_ERROR); 
    unsigned int FBO;
    
    
    glCreateFramebuffers(1, &FBO);
    glNamedFramebufferTexture(FBO, GL_COLOR_ATTACHMENT0, quadTexture, 0);
    glNamedFramebufferDrawBuffer(FBO, GL_COLOR_ATTACHMENT0);
    glNamedFramebufferReadBuffer(FBO, GL_COLOR_ATTACHMENT0);

    unsigned int RBO;
    glCreateRenderbuffers(1, &RBO);
    glNamedRenderbufferStorage(RBO, GL_DEPTH24_STENCIL8, width, height);
    
    glNamedFramebufferRenderbuffer(FBO, GL_DEPTH_STENCIL_ATTACHMENT,GL_RENDERBUFFER, RBO);
    if(glCheckNamedFramebufferStatus(FBO, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;


    glm::vec2 posBounds(-2.0f, 2.0f);
    glm::vec2 zBounds(-13.0f, -11.5f);
    glm::vec2 velBounds(0.5f, 0.8f);
    Particle* particles = initializeRandomParticles(MAX_PARTICLES, posBounds, posBounds, zBounds, velBounds, velBounds, velBounds);
    
    unsigned int dispatch_sizes[3] = {int(ceil(MAX_PARTICLES/256)), 1, 1};
    particlesComputeShader.setExecutionParameters(dispatch_sizes, GL_SHADER_STORAGE_BARRIER_BIT);
    particlesTrailComputeShader.setExecutionParameters(dispatch_sizes, GL_SHADER_STORAGE_BARRIER_BIT);
    
    unsigned int SSBO;
    unsigned int particleVAO;
    auto checkErr = [](const char* label){
    GLenum err = glGetError();
    if(err != GL_NO_ERROR)
        std::cout << "GL Error at " << label << ": " << err << std::endl;
    };

    glCreateBuffers(1, &SSBO);                                                          
    glNamedBufferStorage(SSBO, sizeof(Particle)*MAX_PARTICLES, particles, GL_DYNAMIC_STORAGE_BIT); 
    
    glCreateVertexArrays(1, &particleVAO);  

    // glVertexArrayVertexBuffer(particleVAO, 0, SSBO, 0, sizeof(Particle));               
    // glVertexArrayAttribFormat(particleVAO, 0, 4, GL_FLOAT, GL_FALSE, offsetof(Particle, position));
    // glVertexArrayAttribBinding(particleVAO, 0, 0);
    // glEnableVertexArrayAttrib(particleVAO, 0);                                          
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, SSBO);

    
    std::vector<glm::vec4> trailHistory(MAX_PARTICLES*TRAIL_HISTORY_LENGTH);
    for(int i=0; i<MAX_PARTICLES; i++){
        for(int j=0; j<TRAIL_HISTORY_LENGTH; j++){
            trailHistory[i*TRAIL_HISTORY_LENGTH + j] = particles[i].position;
        }
    }
    unsigned int trailHistorySSBO;
    glCreateBuffers(1, &trailHistorySSBO);
    glNamedBufferStorage(trailHistorySSBO, sizeof(glm::vec4)*MAX_PARTICLES*TRAIL_HISTORY_LENGTH, trailHistory.data(), 0);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, trailHistorySSBO);

    struct CommandBuffer{
        unsigned int count;
        unsigned int primCount;
        unsigned int first;
        unsigned int baseInstance;
    };
    std::vector<CommandBuffer> cmdBuffer(MAX_PARTICLES);
    for(int i=0; i<MAX_PARTICLES; i++){
        cmdBuffer[i].count = TRAIL_HISTORY_LENGTH;
        cmdBuffer[i].primCount = 1;
        cmdBuffer[i].first = i * TRAIL_HISTORY_LENGTH;
        cmdBuffer[i].baseInstance = 0;
    }
    unsigned int cmdBufferSSBO;
    glCreateBuffers(1, &cmdBufferSSBO);
    glNamedBufferStorage(cmdBufferSSBO, sizeof(CommandBuffer)*MAX_PARTICLES, cmdBuffer.data(), 0);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, cmdBufferSSBO);


    struct CameraInformation{
    glm::mat4 projection;
    glm::mat4 view;
    glm::vec3 cameraPos;
    float padding0;
    glm::vec3 cameraFront;
    float padding1;
    };

    CameraInformation camInfo;
    camInfo.projection = cam.getProjectionMatrix();
    camInfo.view = cam.getViewMatrix();
    camInfo.cameraPos = cam.pos;
    camInfo.cameraFront = cam.front;


    unsigned int cameraUBO;
    glCreateBuffers(1, &cameraUBO);
    glNamedBufferStorage(cameraUBO, sizeof(CameraInformation), &camInfo, GL_DYNAMIC_STORAGE_BIT);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, cameraUBO);


    glEnable(GL_PROGRAM_POINT_SIZE);
    
    unsigned int frameCount = 0;
    while (!glfwWindowShouldClose(window))
    {
        glEnable(GL_DEPTH_TEST);
        // std::cout<<"in main loop now\n ";
        float currentFrameTime = static_cast<float>(glfwGetTime());
        // std::cout<<currentFrameTime<<std::endl;
        float deltaT = currentFrameTime - prevFrameTime;
        prevFrameTime = currentFrameTime;
        if(frameCount % 300 == 0) std::cout<<"Frame rate: "<<1.0f/deltaT<<std::endl;

        processInput(window, cam, deltaT*KEYBOARD_MOVE_SPEED);
        
        
        camInfo.projection = cam.getProjectionMatrix();
        camInfo.view = cam.getViewMatrix();
        camInfo.cameraPos = cam.pos;
        camInfo.cameraFront = cam.front;



        particlesComputeShader.use();
        particlesComputeShader.setFloat("deltaTime", deltaT*DT_FACTOR);
        particlesComputeShader.setUint("maxParticles", MAX_PARTICLES);
        particlesComputeShader.execute();
        // glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
        

        // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);

        glNamedBufferSubData(cameraUBO, 0, sizeof(CameraInformation), &camInfo);
        
        glBindFramebuffer(GL_FRAMEBUFFER, FBO);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        
        particlesShader.use();
        particlesShader.setVec3("lightColour", glm::vec3(0.1f, 0.1f, 0.9f));
        glm::mat4 modelMat = glm::mat4(1.0f);
        particlesShader.setMat4("model", modelMat);
    
        glBindVertexArray(particleVAO);
        glDrawArrays(GL_POINTS, 0, MAX_PARTICLES); 
        glBindVertexArray(0);

        particlesTrailComputeShader.use();
        particlesTrailComputeShader.setUint("frameCount", frameCount);
        particlesTrailComputeShader.setUint("maxParticles", MAX_PARTICLES);
        particlesTrailComputeShader.setUint("trailHistoryLength", TRAIL_HISTORY_LENGTH);
        particlesTrailComputeShader.execute();

        particlesTrailShader.use();
        particlesTrailShader.setUint("frameCount", frameCount);
        particlesTrailShader.setUint("trailHistoryLength", TRAIL_HISTORY_LENGTH);
        glBindVertexArray(particleVAO);
        glBindBuffer(GL_DRAW_INDIRECT_BUFFER, cmdBufferSSBO);
        glMultiDrawArraysIndirect(GL_LINE_STRIP, nullptr, MAX_PARTICLES, 0);
        glBindBuffer(GL_DRAW_INDIRECT_BUFFER, 0);
        glBindVertexArray(0);



        
        
       
        

        
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDisable(GL_DEPTH_TEST);
        // glEnable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);
        fbQuadShader.use();
        fbQuadShader.setInt("tex", 0);
        // glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        // glClear(GL_COLOR_BUFFER_BIT);
        fbQuadShader.setFloat("screenWidth", (float)SCREEN_WIDTH);
        fbQuadShader.setFloat("screenHeight", (float)SCREEN_HEIGHT);

        glBindVertexArray(quadVAO);
        // glBindTexture(GL_TEXTURE_2D, quadTexture);
        glBindTextureUnit(0, quadTexture);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glEnable(GL_DEPTH_TEST);

    
        frameCount++;

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}