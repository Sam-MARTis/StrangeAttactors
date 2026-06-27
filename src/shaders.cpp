#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <iostream>

#include "display-objects.hpp"
#include "processing-objects.hpp"
#include "shaders.hpp"
#include "aux.hpp"


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


GraphicsShaders::GraphicsShaders(const std::string vertexShaderFilePath, const std::string fragmentShaderFilePath){
    std::string vertexSource = readSourceFromFile(vertexShaderFilePath);
    std::string fragmentSource = readSourceFromFile(fragmentShaderFilePath);
    const char* vertexShaderCode = vertexSource.c_str();
    const char* fragmentShaderCode = fragmentSource.c_str();
    unsigned int vertexShader, fragmentShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);
    glCompileShader(vertexShader);
    checkCompileErrors(vertexShader, "VERTEX");
    glShaderSource(fragmentShader, 1, &fragmentShaderCode, NULL);
    glCompileShader(fragmentShader); 
    checkCompileErrors(fragmentShader, "FRAGMENT");
    ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

ComputeShaders::ComputeShaders(const std::string computeShaderFilePath){
    std::string computeSource = readSourceFromFile(computeShaderFilePath);
    const char* computeShaderCode = computeSource.c_str();
    unsigned int computeShader;
    computeShader = glCreateShader(GL_COMPUTE_SHADER);
    glShaderSource(computeShader, 1, &computeShaderCode, NULL);
    glCompileShader(computeShader);
    checkCompileErrors(computeShader, "COMPUTE");
    ID = glCreateProgram();
    glAttachShader(ID, computeShader);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");
    glDeleteShader(computeShader);
}

void ComputeShaders::setExecutionParameters(unsigned int workgroup_sizes[3], unsigned int memBarrier){
    for(unsigned int i=0; i<3; i++) this->workgroup_sizes[i] = workgroup_sizes[i];
    this->memBarrier = memBarrier;
}
void ComputeShaders::execute(){
    glDispatchCompute(workgroup_sizes[0], workgroup_sizes[1], workgroup_sizes[2]);
    glMemoryBarrier(memBarrier);
}

void ShaderBase::checkCompileErrors(unsigned int shader, std::string type)
    {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }

void ShaderBase::use(){
    glUseProgram(ID);
}
GLint ShaderBase::getShaderUniformLocation(const std::string& name) const {
    GLint loc;
    std::unordered_map<std::string, GLint>::iterator it = uniforms_loaded.find(name);
    if(it == uniforms_loaded.end()){
        loc = glGetUniformLocation(ID, name.c_str());
        uniforms_loaded[name] = loc;
        if(loc == -1){
            std::cout << "Warning: uniform '" << name << "' not found in shader program " << ID << std::endl;
        }
    }else{
        loc = it->second;
    }
    return loc;

}
void ShaderBase::setBool(const std::string &name, bool value) const{
    glUniform1i(getShaderUniformLocation(name), (int)value); 
}
void ShaderBase::setInt(const std::string &name, int value) const{
    glUniform1i(getShaderUniformLocation(name), value); 
}
void ShaderBase::setUint(const std::string &name, int unsigned value) const{
    glUniform1ui(getShaderUniformLocation(name), value); 
}
void ShaderBase::setFloat(const std::string &name, float value) const{
    glUniform1f(getShaderUniformLocation(name), value); 
}
void ShaderBase::setVec2(const std::string &name, glm::vec2 value) const{
    glUniform2f(getShaderUniformLocation(name), value.x, value.y); 
}
void ShaderBase::setVec3(const std::string &name, glm::vec3 value) const{
    glUniform3f(getShaderUniformLocation(name), value.x, value.y, value.z); 
}
void ShaderBase::setMat4(const std::string &name, glm::mat4 mat) const{
    glUniformMatrix4fv(getShaderUniformLocation(name), 1, GL_FALSE, &mat[0][0]);    
}

