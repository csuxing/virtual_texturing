#include "virtual_texture.h"
#include <algorithm>
#include <stdexcept>
#include <chrono>

VirtualTexture::VirtualTexture(int virtualSize, int pageSize, int physicalSize)
    : virtualSize_(virtualSize)
    , pageSize_(pageSize)
    , physicalSize_(physicalSize) {
    initializeTextures();

    // 初始化可用物理槽位
    int numSlots = physicalSize_ / pageSize_;
    for (int y = 0; y < numSlots; ++y) {
        for (int x = 0; x < numSlots; ++x) {
            freePhysicalSlots_.push_back(glm::ivec2(x, y));
        }
    }
}

VirtualTexture::~VirtualTexture() {
    glDeleteTextures(1, &pageTableTexture_);
    glDeleteTextures(1, &physicalTexture_);
}

void VirtualTexture::initializeTextures() {
    // 创建页表纹理
    glGenTextures(1, &pageTableTexture_);
    glBindTexture(GL_TEXTURE_2D, pageTableTexture_);
    int numPages = virtualSize_ / pageSize_;
    std::vector<glm::ivec4> pageTable(numPages * numPages, glm::ivec4(-1));
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32I, numPages, numPages, 0, GL_RGBA_INTEGER, GL_INT, pageTable.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // 创建物理纹理
    glGenTextures(1, &physicalTexture_);
    glBindTexture(GL_TEXTURE_2D, physicalTexture_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, physicalSize_, physicalSize_, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void VirtualTexture::bindPageTable(GLuint unit) {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, pageTableTexture_);
}

void VirtualTexture::bindPhysicalTexture(GLuint unit) {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, physicalTexture_);
}

void VirtualTexture::updatePageTable() {
    int numPages = virtualSize_ / pageSize_;
    std::vector<glm::ivec4> pageTable(numPages * numPages, glm::ivec4(-1));

    for (const auto& pair : pages_) {
        const auto& page = pair.second;
        if (page->loaded) {
            int index = page->position.y * numPages + page->position.x;
            pageTable[index] = glm::ivec4(page->physicalPosition.x, page->physicalPosition.y,
                                         page->mipLevel, 1);
        }
    }

    glBindTexture(GL_TEXTURE_2D, pageTableTexture_);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, numPages, numPages, GL_RGBA_INTEGER, GL_INT, pageTable.data());
}

void VirtualTexture::requestPage(const glm::ivec2& position, int mipLevel) {
    auto it = pages_.find(position);
    if (it != pages_.end()) {
        it->second->lastUsed = std::chrono::system_clock::now().time_since_epoch().count();
        return;
    }

    auto page = std::make_unique<Page>();
    page->position = position;
    page->mipLevel = mipLevel;
    page->loaded = false;
    page->lastUsed = std::chrono::system_clock::now().time_since_epoch().count();

    if (!freePhysicalSlots_.empty()) {
        page->physicalPosition = allocatePhysicalSlot();
        page->loaded = true;
    } else {
        // 查找最久未使用的页面
        auto lru = std::min_element(pages_.begin(), pages_.end(),
            [](const auto& a, const auto& b) {
                return a.second->lastUsed < b.second->lastUsed;
            });

        if (lru != pages_.end() && lru->second->lastUsed < page->lastUsed) {
            page->physicalPosition = lru->second->physicalPosition;
            unloadPage(*lru->second);
            pages_.erase(lru);
            page->loaded = true;
        }
    }

    pages_[position] = std::move(page);
}

void VirtualTexture::loadPage(const Page& page, const void* data) {
    if (!page.loaded) return;

    glBindTexture(GL_TEXTURE_2D, physicalTexture_);
    glTexSubImage2D(GL_TEXTURE_2D, 0,
                    page.physicalPosition.x * pageSize_,
                    page.physicalPosition.y * pageSize_,
                    pageSize_, pageSize_,
                    GL_RGBA, GL_UNSIGNED_BYTE, data);
}

void VirtualTexture::unloadPage(const Page& page) {
    if (!page.loaded) return;
    freePhysicalSlot(page.physicalPosition);
}

glm::ivec2 VirtualTexture::allocatePhysicalSlot() {
    if (freePhysicalSlots_.empty()) {
        throw std::runtime_error("No free physical slots available");
    }

    glm::ivec2 slot = freePhysicalSlots_.back();
    freePhysicalSlots_.pop_back();
    return slot;
}

void VirtualTexture::freePhysicalSlot(const glm::ivec2& slot) {
    freePhysicalSlots_.push_back(slot);
}