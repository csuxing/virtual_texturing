#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>

class TextureManager {
public:
    TextureManager();
    ~TextureManager();

    // 加载并创建页表纹理
    GLuint createPageTable(int width, int height);
    
    // 加载并创建物理纹理
    GLuint createPhysicalTexture(int width, int height);
    
    // 更新页表纹理
    void updatePageTable(const glm::ivec2& position, const glm::ivec4& data);
    
    // 更新物理纹理区域
    void updatePhysicalTexture(const glm::ivec2& position, int width, int height, const void* data);

private:
    GLuint pageTableTexture;
    GLuint physicalTexture;
};