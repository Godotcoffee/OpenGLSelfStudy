#include <string>
#include "StringShader.hpp"

class FileShader : public StringShader
{
public:
    FileShader(const std::string &vertexFile, const std::string &fragmentFile);
};