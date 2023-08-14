#include "AbstractShader.hpp"

#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

AbstractShader::~AbstractShader() {}

void AbstractShader::use() const {
    glUseProgram(_shaderId);
}

unsigned int AbstractShader::getId() const
{
    return _shaderId;
}

GLint AbstractShader::getUniformLoc(const std::string &name) const
{
    return glGetUniformLocation(_shaderId, name.c_str());
}

void AbstractShader::setBool(const std::string &name, bool value) const
{
    glUniform1i(getUniformLoc(name), (int) value);
}

void AbstractShader::setInt(const std::string &name, int value) const
{
    glUniform1i(getUniformLoc(name), value);
}

void AbstractShader::setFloat(const std::string &name, float value) const
{
    glUniform1f(getUniformLoc(name), value);
}

void AbstractShader::setVec2(const std::string &name, const glm::vec2 &value) const
{
    glUniform2fv(getUniformLoc(name), 1, glm::value_ptr(value));
}

void AbstractShader::setVec2(const std::string &name, float x, float y) const
{
    glUniform2f(getUniformLoc(name), x, y);
}

void AbstractShader::setVec3(const std::string &name, const glm::vec3 &value) const
{
    glUniform3fv(getUniformLoc(name), 1, glm::value_ptr(value));
}

void AbstractShader::setVec3(const std::string &name, float x, float y, float z) const
{
    glUniform3f(getUniformLoc(name), x, y, z);
}

void AbstractShader::setVec4(const std::string &name, const glm::vec4 &value) const
{
    glUniform4fv(getUniformLoc(name), 1, glm::value_ptr(value));
}

void AbstractShader::setVec4(const std::string &name, float x, float y, float z, float w) const
{
    glUniform4f(getUniformLoc(name), x, y, z, w);
}

void AbstractShader::setMat2(const std::string &name, const glm::mat2 &mat) const
{
    glUniformMatrix2fv(getUniformLoc(name), 1, GL_FALSE, glm::value_ptr(mat));
}

void AbstractShader::setMat3(const std::string &name, const glm::mat3 &mat) const
{
    glUniformMatrix3fv(getUniformLoc(name), 1, GL_FALSE, glm::value_ptr(mat));
}

void AbstractShader::setMat4(const std::string &name, const glm::mat4 &mat) const
{
    glUniformMatrix4fv(getUniformLoc(name), 1, GL_FALSE, glm::value_ptr(mat));
}
