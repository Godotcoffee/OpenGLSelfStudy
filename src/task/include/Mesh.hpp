#ifndef Mesh_HPP
#define Mesh_HPP

#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "AbstractShader.hpp"

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

struct Texture {
    unsigned int id;
    std::string type;
};

class Mesh {
private:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    unsigned int VAO, VBO, EBO;
public:
    Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices, const std::vector<Texture> &textures);
    void setupMesh();
    void draw(const AbstractShader &shader) const;
};

#endif