#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>

class TextureManager {
public:
    TextureManager();
    ~TextureManager();

    // ���ز�����ҳ������
    GLuint createPageTable(int width, int height);
    
    // ���ز�������������
    GLuint createPhysicalTexture(int width, int height);
    
    // ����ҳ������
    void updatePageTable(const glm::ivec2& position, const glm::ivec4& data);
    
    // ����������������
    void updatePhysicalTexture(const glm::ivec2& position, int width, int height, const void* data);

private:
    GLuint pageTableTexture;
    GLuint physicalTexture;
};