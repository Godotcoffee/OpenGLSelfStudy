#ifndef StringShader_HPP
#define StringShader_HPP

#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "AbstractShader.hpp"

class StringShader : public AbstractShader
{
protected:
    StringShader();
    void init(const char *vertexShaderSource, const char *fragmentShaderSource);
public:
    StringShader(const std::string &vertexShaderSource, const std::string &fragmentShaderSource);
    virtual ~StringShader() = default;
};

#endif