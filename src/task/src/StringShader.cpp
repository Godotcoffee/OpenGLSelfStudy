#include "StringShader.hpp"
#include <iostream>
#include <glad/glad.h>

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
