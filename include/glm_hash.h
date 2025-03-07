#pragma once
#include <glm/glm.hpp>

namespace std {
    template<>
    struct hash<glm::ivec2> {
        size_t operator()(const glm::ivec2& v) const {
            size_t seed = 0;
            hash<int> hasher;
            seed ^= hasher(v.x) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            seed ^= hasher(v.y) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            return seed;
        }
    };
}