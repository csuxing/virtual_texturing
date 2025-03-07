#include "texture_manager.h"
#include <iostream>

TextureManager::TextureManager() : pageTableTexture(0), physicalTexture(0) {}

TextureManager::~TextureManager() {
    if (pageTableTexture) glDeleteTextures(1, &pageTableTexture);
    if (physicalTexture) glDeleteTextures(1, &physicalTexture);
}

GLuint TextureManager::createPageTable(int width, int height) {
    glGenTextures(1, &pageTableTexture);
    glBindTexture(GL_TEXTURE_2D, pageTableTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32I, width, height, 0, GL_RGBA_INTEGER, GL_INT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    return pageTableTexture;
}

GLuint TextureManager::createPhysicalTexture(int width, int height) {
    glGenTextures(1, &physicalTexture);
    glBindTexture(GL_TEXTURE_2D, physicalTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    return physicalTexture;
}

void TextureManager::updatePageTable(const glm::ivec2& position, const glm::ivec4& data) {
    glBindTexture(GL_TEXTURE_2D, pageTableTexture);
    glTexSubImage2D(GL_TEXTURE_2D, 0, position.x, position.y, 1, 1, GL_RGBA_INTEGER, GL_INT, &data[0]);
}

void TextureManager::updatePhysicalTexture(const glm::ivec2& position, int width, int height, const void* data) {
    glBindTexture(GL_TEXTURE_2D, physicalTexture);
    glTexSubImage2D(GL_TEXTURE_2D, 0, position.x, position.y, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
}