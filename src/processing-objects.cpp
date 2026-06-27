#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <iostream>

#include "display-objects.hpp"
#include "processing-objects.hpp"
#include "aux.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>



Mesh::Mesh(std::vector<Vertex> vertices,std::vector<unsigned int> indices, std::vector<Texture> textures){
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;
    setupMesh();
}

void Mesh::setupMesh(){


    glCreateVertexArrays(1, &VAO);
    glCreateBuffers(1, &VBO);
    glCreateBuffers(1, &EBO);

    glNamedBufferStorage(VBO, vertices.size()*sizeof(Vertex), vertices.data(), 0); // 0-> data is gpu-resident and immuatbble by cpu
    glNamedBufferStorage(EBO, indices.size()*sizeof(unsigned int), indices.data(), 0);

    glVertexArrayVertexBuffer(VAO, 0, VBO, 0, sizeof(Vertex));
    glVertexArrayElementBuffer(VAO, EBO);

    glVertexArrayAttribFormat(VAO, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, Position));
    glVertexArrayAttribFormat(VAO, 1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, Normal));
    glVertexArrayAttribFormat(VAO, 2, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, TexCoords));

    glVertexArrayAttribBinding(VAO, 0, 0);
    glVertexArrayAttribBinding(VAO, 1, 0);
    glVertexArrayAttribBinding(VAO, 2, 0);

    glEnableVertexArrayAttrib(VAO, 0);
    glEnableVertexArrayAttrib(VAO, 1);
    glEnableVertexArrayAttrib(VAO, 2);

}
// GLuint Mesh::getUniformLocationCache(const std::string& name) const {
//     GLuint loc;
//     std::unordered_map<std::string, unsigned int>::iterator it = uniform_locations.find(name);
//     if(it == uniform_locations.end()){
//         loc = glGetUniformLocation(ID, name.c_str());
//         uniform_locations[name] = loc;
//     }else{
//         loc = it->second;
//     }
//     return loc;

// }
void Mesh::draw(GraphicsShaders& shader){
    unsigned int diffTextureCount = 1;
    unsigned int specTextureCount = 1;
    for(unsigned int i=0; i<textures.size(); i++){
        // glActiveTexture(GL_TEXTURE0 + i);
        std::string number;
        std::string name = textures[i].type;
        if(name=="texture_diffuse"){
            number = std::to_string(diffTextureCount);
            diffTextureCount+=1;
        }else{
            if(name=="texture_specular"){
                number = std::to_string(specTextureCount);
                specTextureCount+=1;
            }else{
                std::cout<<"ERROR::MESH DRAW::Unknown texture type: "<<name<<std::endl;
            }
        }
        glBindTextureUnit(i, textures[i].id);
        GLuint loc = shader.getShaderUniformLocation(name+number);
        glUniform1i(loc, i);
        // glUniform1i(glGetUniformLocation(shader.ID, (name+number).c_str()), i);
        // glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }
    glBindVertexArray(VAO);
    glDrawElements(GL_POINTS, static_cast<unsigned int>(indices.size()),GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    // glActiveTexture(GL_TEXTURE0);

}


Model::Model(const std::string& path){
    std::cout<<"Model initializing with filepath:"<<path<<std::endl;
    loadModel(path);
    std::cout<<"COMPLETE::INITIALIZATION::Model loaded from path";
};

void Model::draw(GraphicsShaders &shader){
    for(unsigned int i=0; i<meshes.size(); i++){
        meshes[i].draw(shader);
    }

}

void Model::loadModel(const std::string& path){
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
        std::cout<< "ERROR::ASSIMP:: " << importer.GetErrorString() <<std::endl;
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));
    std::cout<<"Loading model from directory: "<<directory<<std::endl;
    processNode(scene->mRootNode, scene, 0);
}

void Model::processNode(aiNode* node, const aiScene* scene, unsigned int recursionDepth){
    // std::cout<<"Processing node depth "<<recursionDepth<<" with "<<node->mNumMeshes<<" meshes and "<<node->mNumChildren<<" children\n";
    // std::cout<<"Node name: "<<node->mName.C_Str()<<" depth: "<<recursionDepth<<" meshes: "<<node->mNumMeshes<<" children: "<<node->mNumChildren<<std::endl;
    for(unsigned int i=0; i<node->mNumMeshes; i++){
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }
    for(unsigned int i=0; i<node->mNumChildren; i++){
        processNode(node->mChildren[i], scene, recursionDepth + 1);
    }
    // std::cout<<"Finished processing node depth "<<recursionDepth<<" with "<<node->mNumMeshes<<" meshes and "<<node->mNumChildren<<" children\n";
}
Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene){
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    const bool mesh_has_normals = mesh->HasNormals();
    bool mesh_has_textures = false;
    if(mesh->mTextureCoords[0]) mesh_has_textures=true;
    for(unsigned int i=0; i<mesh->mNumVertices; i++){
        
        Vertex vertex;
        glm::vec3 vector;
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;
        if(mesh_has_normals){
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;
        }
        if(mesh_has_textures){
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
        }else{
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        }
        vertices.push_back(vertex);
        
    }
    for(unsigned int i=0; i<mesh->mNumFaces;i++){
        aiFace face = mesh->mFaces[i];

        for(unsigned int j=0; j<face.mNumIndices; j++){
            indices.push_back(face.mIndices[j]);
        }
    }

    aiMaterial* material = scene -> mMaterials[mesh->mMaterialIndex];

    std::vector<Texture> diffuseMaps = loadMaterialTexture(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

    std::vector<Texture> specularMaps = loadMaterialTexture(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());


    return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTexture(aiMaterial *mat, aiTextureType type, std::string typeName){
    std::vector<Texture> textures;
    for(unsigned int i=0; i<mat->GetTextureCount(type); i++){
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;
        for(unsigned int j=0; j<textures_loaded.size(); j++){
            if(std::strcmp(textures_loaded[j].path.data(), str.C_Str())==0){
                skip = true;
                break;
            }
        }
        if(!skip){
            Texture texture;
            std::string filePath = directory +std::string("/")+ std::string(str.C_Str());
            int width, height, nChannels;
            texture.id = readTextureFromFile(filePath, width, height, nChannels);
            texture.path = str.C_Str();
            texture.type = typeName;
            textures.push_back(texture);
            textures_loaded.push_back(texture);
        }
    }
    return textures;
}


