#include "Model.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include <queue>
#include <vector>
#include <unordered_map>
#include <stb_image.h>

Model::Model(const std::string &path)
{
    loadModel(path);
}

void Model::draw(const AbstractShader &shader) const
{
    for (auto &mesh : _mesh) {
        mesh.draw(shader);
    }
}

void Model::loadModel(const std::string &path)
{
    Assimp::Importer importer;
    std::cout << "reading model..." << std::endl;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
    std::cout << "done" << std::endl;
    if (!scene || scene->mFlags && AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }

    _directory = path.substr(0, path.find_last_of('/'));
    std::cout << "processing model..." << std::endl;
    processNode(*scene);
    std::cout << "done" << std::endl;
}

void Model::processNode(const aiScene &scene)
{
    std::queue<aiNode *> que;

    que.push(scene.mRootNode);

    while (!que.empty()) {
        aiNode *node = que.front(); que.pop();
        if (node) {
            for (std::size_t i = 0; i < node->mNumMeshes; ++i) {
                auto &mesh = scene.mMeshes[node->mMeshes[i]];
                _mesh.emplace_back(processMesh(*mesh, scene));
            }
            for (std::size_t i = 0; i < node->mNumChildren; ++i) {
                que.push(node->mChildren[i]);
            }
        }
    }
}

Mesh Model::processMesh(const aiMesh &mesh, const aiScene &scene) 
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    for (std::size_t i = 0; i < mesh.mNumVertices; ++i) {
        auto &meshVertex = mesh.mVertices[i];
        auto &meshNormal = mesh.mNormals[i];
        
        Vertex vertex;
        
        vertex.position = glm::vec3(meshVertex.x, meshVertex.y, meshVertex.z);
        vertex.normal = glm::vec3(meshNormal.x, meshNormal.y, meshNormal.z);

        if (mesh.mTextureCoords[0]) {
            auto &meshTextCoords = mesh.mTextureCoords[0][i];
            vertex.texCoords = glm::vec2(meshTextCoords.x, meshTextCoords.y);
        } else {
            vertex.texCoords = glm::vec2(0.0f, 0.0f);
        }

        vertices.emplace_back(vertex);
    }

    for (std::size_t i = 0; i < mesh.mNumFaces; ++i) {
        auto &face = mesh.mFaces[i];
        for (std::size_t j = 0; j < face.mNumIndices; ++j) {
            indices.emplace_back(face.mIndices[j]);
        }
    }

    auto &aiMaterial = *scene.mMaterials[mesh.mMaterialIndex];

    loadMaterialTextures(textures, aiMaterial, aiTextureType_DIFFUSE, "texture_diffuse");
    loadMaterialTextures(textures, aiMaterial, aiTextureType_SPECULAR, "texture_specular");

    return {vertices, indices, textures};
}

void Model::loadMaterialTextures(std::vector<Texture> &textures, const aiMaterial &material, aiTextureType type, const std::string typeName)
{
    for (std::size_t i = 0; i < material.GetTextureCount(type); ++i) {
        aiString str;
        material.GetTexture(type, i, &str);
        std::string sstr = str.C_Str();
        Texture texture;
        if (_loadedTexture.count(sstr) > 0) {
            texture.id = _loadedTexture[sstr];
        } else {
            texture.id = TextureFromFile(sstr, _directory);
            _loadedTexture[sstr] = texture.id;
        }
        texture.type = typeName;

        textures.emplace_back(texture);
    }
}

unsigned int Model::TextureFromFile(const std::string &file, const std::string &dir)
{
    std::string path = dir + "/" + file;

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;

    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

    GLenum format;
    switch (nrChannels)
    {
    case 1:
        format = GL_RED;
        break;
    case 3:
        format = GL_RGB;
        break;
    case 4:
        format = GL_RGBA;
        break;
    default:
        format = GL_RED;
        break;
    }

    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    return texture;
}