
#pragma once 

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <vector>
#include <unordered_map>

#include "display-objects.hpp"
#include "shaders.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>



struct Vertex{
    glm::vec3 Position; // try later replacing the attribpointer format with offsetof macro for offset calc
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct Texture{
    unsigned int id;
    std::string type;
    std::string path;
};

class Mesh{
    private:
    unsigned int VBO, EBO, VAO;
    void setupMesh();

    
    public:
    std::vector<Vertex> vertices;
    std::vector<Texture> textures;
    std::vector<unsigned int> indices;
    Mesh(std::vector<Vertex> vertices,std::vector<unsigned int> indices, std::vector<Texture> textures);
    void draw(GraphicsShaders& shader);

};


class Model{
    public:
    std::vector<Texture> textures_loaded;
    std::vector<Mesh> meshes;
    std::string directory;
    Model(const std::string &path);
    void draw(GraphicsShaders& shader);

    private:
    void loadModel(const std::string& path);
    void processNode(aiNode* node, const aiScene* scene, unsigned int recursionDepth=0);
    std::vector<Texture> loadMaterialTexture(aiMaterial *mat, aiTextureType type, std::string typeName);
    Mesh processMesh(aiMesh *mesh, const aiScene* scene);
};

