#include "FileShader.hpp"
#include <string>
#include <fstream>
#include <iostream>
#include "sstream"

FileShader::FileShader(const std::string &vertexFile, const std::string &fragmentFile) : StringShader() {
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        vShaderFile.open(vertexFile);
        fShaderFile.open(fragmentFile);

        std::stringstream vShaderStream, fShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        vShaderFile.close();
        fShaderFile.close();

        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    } catch(const std::ifstream::failure& e) {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ::IFSTREAM_FAILURE" << "\n"
                  << "Details: " << e.what() << std::endl;
    } catch(const std::system_error& e) {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ::SYSTEM_ERROR" << "\n"
                  << "Details: " << e.what() << std::endl;
    }
    const char *vertexSrcAddr = vertexCode.c_str();
    const char *fragmentSrcAddr = fragmentCode.c_str();
    init(vertexSrcAddr, fragmentSrcAddr);
}
