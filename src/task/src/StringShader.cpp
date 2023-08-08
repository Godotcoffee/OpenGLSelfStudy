#include "StringShader.hpp"
#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

void StringShader::init(const char *vertexShaderSource, const char *fragmentShaderSource)
{
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    int success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    _shaderId = glCreateProgram();
    glAttachShader(_shaderId, vertexShader);
    glAttachShader(_shaderId, fragmentShader);
    glLinkProgram(_shaderId);

    glGetProgramiv(_shaderId, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(_shaderId, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n"
                  << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

StringShader::StringShader() : _shaderId(0)
{
}

StringShader::StringShader(const std::string &vertexShaderSource, const std::string &fragmentShaderSource) : _shaderId(0)
{
    const char *vertexSrcAddr = vertexShaderSource.c_str();
    const char *fragmentSrcAddr = fragmentShaderSource.c_str();
    init(vertexSrcAddr, fragmentSrcAddr);
}

void StringShader::use() const {
    glUseProgram(_shaderId);
}

unsigned int StringShader::getId() const
{
    return _shaderId;
}

GLint StringShader::getUniformLoc(const std::string &name) const
{
    return glGetUniformLocation(_shaderId, name.c_str());
}

void StringShader::setBool(const std::string &name, bool value) const
{
    glUniform1i(getUniformLoc(name), (int) value);
}

void StringShader::setInt(const std::string &name, int value) const
{
    glUniform1i(getUniformLoc(name), value);
}

void StringShader::setFloat(const std::string &name, float value) const
{
    glUniform1f(getUniformLoc(name), value);
}

void StringShader::setVec2(const std::string &name, const glm::vec2 &value) const
{
    glUniform2fv(getUniformLoc(name), 1, glm::value_ptr(value));
}

void StringShader::setVec2(const std::string &name, float x, float y) const
{
    glUniform2f(getUniformLoc(name), x, y);
}

void StringShader::setVec3(const std::string &name, const glm::vec3 &value) const
{
    glUniform3fv(getUniformLoc(name), 1, glm::value_ptr(value));
}

void StringShader::setVec3(const std::string &name, float x, float y, float z) const
{
    glUniform3f(getUniformLoc(name), x, y, z);
}

void StringShader::setVec4(const std::string &name, const glm::vec4 &value) const
{
    glUniform4fv(getUniformLoc(name), 1, glm::value_ptr(value));
}

void StringShader::setVec4(const std::string &name, float x, float y, float z, float w) const
{
    glUniform4f(getUniformLoc(name), x, y, z, w);
}

void StringShader::setMat2(const std::string &name, const glm::mat2 &mat) const
{
    glUniformMatrix2fv(getUniformLoc(name), 1, GL_FALSE, glm::value_ptr(mat));
}

void StringShader::setMat3(const std::string &name, const glm::mat3 &mat) const
{
    glUniformMatrix3fv(getUniformLoc(name), 1, GL_FALSE, glm::value_ptr(mat));
}

void StringShader::setMat4(const std::string &name, const glm::mat4 &mat) const
{
    glUniformMatrix4fv(getUniformLoc(name), 1, GL_FALSE, glm::value_ptr(mat));
}
