#ifndef MODEL_HPP
#define MODEL_HPP

#include <vector>
#include <string>
#include <unordered_map>
#include "assimp/scene.h"
#include "Mesh.hpp"
#include "AbstractShader.hpp"

class Model 
{
private:
    std::vector<Mesh> _mesh;
    std::string _directory;
    std::unordered_map<std::string, unsigned int> _loadedTexture;

    void loadModel(const std::string &path);
    void processNode(const aiScene &scene);
    Mesh processMesh(const aiMesh &mesh, const aiScene &scene);
    void loadMaterialTextures(std::vector<Texture> &textures, const aiMaterial &material, aiTextureType type, const std::string typeName);
    unsigned int TextureFromFile(const std::string &file, const std::string &dir);
public:
    Model(const std::string &path);
    void draw(const AbstractShader &shader) const;
};

#endif