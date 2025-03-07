#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <unordered_map>
#include <memory>
#include "glm_hash.h"
class VirtualTexture {
public:
    struct Page {
        glm::ivec2 position;  // 页面在虚拟纹理中的位置
        glm::ivec2 physicalPosition;  // 页面在物理纹理中的位置
        int mipLevel;  // MIP级别
        bool loaded;   // 是否已加载
        uint64_t lastUsed;  // 最后使用时间
    };

    VirtualTexture(int virtualSize, int pageSize, int physicalSize);
    ~VirtualTexture();

    void bindPageTable(GLuint unit);
    void bindPhysicalTexture(GLuint unit);
    void updatePageTable();
    void requestPage(const glm::ivec2& position, int mipLevel);
    void loadPage(const Page& page, const void* data);
    void unloadPage(const Page& page);

private:
    int virtualSize_;   // 虚拟纹理大小
    int pageSize_;      // 页面大小
    int physicalSize_;  // 物理纹理大小

    GLuint pageTableTexture_;    // 页表纹理
    GLuint physicalTexture_;     // 物理纹理
    
    std::unordered_map<glm::ivec2, std::unique_ptr<Page>> pages_;  // 页面映射
    std::vector<glm::ivec2> freePhysicalSlots_;  // 可用物理槽位

    void initializeTextures();
    glm::ivec2 allocatePhysicalSlot();
    void freePhysicalSlot(const glm::ivec2& slot);
};