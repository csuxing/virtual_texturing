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
        glm::ivec2 position;  // ҳ�������������е�λ��
        glm::ivec2 physicalPosition;  // ҳ�������������е�λ��
        int mipLevel;  // MIP����
        bool loaded;   // �Ƿ��Ѽ���
        uint64_t lastUsed;  // ���ʹ��ʱ��
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
    int virtualSize_;   // ���������С
    int pageSize_;      // ҳ���С
    int physicalSize_;  // ���������С

    GLuint pageTableTexture_;    // ҳ������
    GLuint physicalTexture_;     // ��������
    
    std::unordered_map<glm::ivec2, std::unique_ptr<Page>> pages_;  // ҳ��ӳ��
    std::vector<glm::ivec2> freePhysicalSlots_;  // ���������λ

    void initializeTextures();
    glm::ivec2 allocatePhysicalSlot();
    void freePhysicalSlot(const glm::ivec2& slot);
};