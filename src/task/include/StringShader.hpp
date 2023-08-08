#ifndef StringShader_HPP
#define StringShader_HPP

#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>

class StringShader
{
private:
    unsigned int _shaderId;
protected:
    StringShader();
    void init(const char *vertexShaderSource, const char *fragmentShaderSource);
public:
    StringShader(const std::string &vertexShaderSource, const std::string &fragmentShaderSource);
    virtual ~StringShader() = default;

    void use() const;
    unsigned int getId() const;
    
    GLint getUniformLoc(const std::string &name) const;

    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setVec2(const std::string &name, const glm::vec2 &value) const;
    void setVec2(const std::string &name, float x, float y) const;
    void setVec3(const std::string &name, const glm::vec3 &value) const;
    void setVec3(const std::string &name, float x, float y, float z) const;
    void setVec4(const std::string &name, const glm::vec4 &value) const;
    void setVec4(const std::string &name, float x, float y, float z, float w) const;
    void setMat2(const std::string &name, const glm::mat2 &mat) const;
    void setMat3(const std::string &name, const glm::mat3 &mat) const;
    void setMat4(const std::string &name, const glm::mat4 &mat) const;
};

#endif