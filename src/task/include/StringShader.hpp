#ifndef StringShader_HPP
#define StringShader_HPP

#include <string>

class StringShader
{
private:
    unsigned int _shaderId;
protected:
    StringShader();
    void init(const char *vertexShaderSource, const char *fragmentShaderSource);
public:
    StringShader(const std::string &vertexShaderSource, const std::string &fragmentShaderSource);
    void use() const;
    unsigned int getId() const;
    virtual ~StringShader() = default;
};

#endif