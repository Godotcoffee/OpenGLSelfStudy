#ifndef StringShader_HPP
#define StringShader_HPP

#include <string>
#include <glad/glad.h>

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
};

#endif