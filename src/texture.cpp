// Copyright 2016 Lauri Gustafsson
/*  This file is part of lib3dapp.

    lib3dapp is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    lib3dapp is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with lib3dapp.  If not, see <http://www.gnu.org/licenses/>.*/

#include "3dapp_texture.hpp"
#include <iostream>
#include "3dapp_consts.hpp"

using namespace Lib3dapp;

Texture::Texture(RgbaImage image):
width(image.getWidth()),
height(image.getHeight()),
filter(GL_NEAREST) {
    GLuint format;
    if (image.hasAlpha() && image.hasColor())
        format = GL_RGBA;
    else if (!image.hasAlpha() && image.hasColor())
        format = GL_RGB;
    else if (image.hasAlpha() && !image.hasColor())
        #ifdef RASPI_BUILD
            format = GL_LUMINANCE;
        #else
            format = GL_RED;
        #endif
    else {
        std::cout << "Texture tried to load a broken RgbaImage!\n";
        exit(ERR_TEXTURE);
    }
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &handle);
    glBindTexture(GL_TEXTURE_2D, handle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
    
    //Needed to avoid issues with weird image sizes
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    #ifndef RASPI_BUILD
        glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
        glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
        glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
    #endif
    
    glTexImage2D(GL_TEXTURE_2D, 0, format, image.getWidth(), image.getHeight(), 0, format, GL_UNSIGNED_BYTE, image.getArray());
    std::cout << "Texture loaded. " << format << " " << image.getWidth() << " " << image.getHeight() << " " << (image.hasAlpha() ? "alpha" : "no alpha") << std::endl;
    std::cout << "GL_RGBA=" << GL_RGBA << " GL_RGB=" << GL_RGB << std::endl;
}

Texture::Texture(GLuint w, GLuint h):
width(w),
height(h) {
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &handle);
    glBindTexture(GL_TEXTURE_2D, handle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
}

Texture::~Texture() {
    glDeleteTextures(1, &handle);
}

GLuint Texture::getHandle() {
    return handle;
}

void Texture::bindToUnit(GLuint unit) {
    glActiveTexture(GL_TEXTURE0+unit);
    glBindTexture(GL_TEXTURE_2D, handle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
}

GLuint Texture::getWidth() {
    return width;
}

GLuint Texture::getHeight() {
    return height;
}

void Texture::setFilter(GLuint _filter) {
	filter = _filter;
}

float Texture::getAspect() {
    return ((1.0f*width)/height);
}
