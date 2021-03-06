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

#include "3dapp_renderbuffer.hpp"

using namespace Lib3dapp;

Renderbuffer::Renderbuffer(GLuint w, GLuint h) {
    glGenRenderbuffers(1, &handle);
    glBindRenderbuffer(GL_RENDERBUFFER, handle);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, w, h);
}

Renderbuffer::~Renderbuffer() {
    glDeleteRenderbuffers(1, &handle);
}

GLuint Renderbuffer::getHandle() {
    return handle;
}

void Renderbuffer::bind() {
    glBindRenderbuffer(GL_RENDERBUFFER, handle);
}
