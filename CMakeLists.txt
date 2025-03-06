cmake_minimum_required(VERSION 3.10)
project(virtual_texturing)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找OpenGL包
find_package(OpenGL REQUIRED)

# 添加GLFW和GLAD
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

# 添加子目录
add_subdirectory(external/glfw)
add_subdirectory(external/glad)

# 创建可执行文件
add_executable(virtual_texturing
    src/main.cpp
    src/virtual_texture.cpp
    src/shader.cpp
    src/texture_manager.cpp
)

# 包含目录
target_include_directories(virtual_texturing PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/external/glfw/include
    ${CMAKE_SOURCE_DIR}/external/glad/include
)

# 链接库
target_link_libraries(virtual_texturing PRIVATE
    OpenGL::GL
    glfw
    glad
)