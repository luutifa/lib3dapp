#include "paths.hpp"
#include <string>

std::string meshPath(std::string filename) {
    std::string tmp = DATA_DIR;
    tmp+="/";
    tmp+=MESH_DIR;
    tmp+="/";
    tmp+=filename;
    return tmp;
}

std::string texturePath(std::string filename) {
    std::string tmp = DATA_DIR;
    tmp+="/";
    tmp+=TEXTURE_DIR;
    tmp+="/";
    tmp+=filename;
    return tmp;
}

std::string shaderPath(std::string name) {
    std::string tmp = DATA_DIR;
    tmp += "/";
    tmp += SHADER_DIR;
    tmp += "/";
    tmp += name;
    return tmp;
}