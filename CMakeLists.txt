cmake_minimum_required(VERSION 3.23)
project(virtual_texturing)

# 设置C++标准
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找OpenGL包
find_package(OpenGL REQUIRED)

# 添加 GLM 编译定义
# GLM 配置
add_definitions(
    -DGLM_FORCE_RADIANS
    -DGLM_FORCE_DEPTH_ZERO_TO_ONE
    -DGLM_ENABLE_EXPERIMENTAL
    -DGLM_FORCE_CXX14
)

#add_definitions(GLM_COMPILER)
#target_compile_definitions(GLM_COMPILER)
# 添加GLFW和GLAD
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

# 添加子目录
add_subdirectory(external/glfw)
#add_subdirectory(external/glad)

set(SOURCES
    src/main.cpp
    src/virtual_texture.cpp
    src/shader.cpp
    #src/texture_manager.cpp 
)

set(HEADERS
    include/shader.h
    include/virtual_texture.h
    include/glm_hash.h
    include/texture_manager.h
)

# 添加 glad 源文件
set(GLAD_SOURCE ${CMAKE_SOURCE_DIR}/external/glad/src/glad.c)

set(SOURCES
    src/main.cpp
    src/virtual_texture.cpp
    src/shader.cpp
    src/texture_manager.cpp
    ${GLAD_SOURCE}    # 添加 glad 源文件
)

set(SHADERS
    shaders/vertex.glsl
    shaders/fragment.glsl
)

# 创建可执行文件
add_executable(virtual_texturing
    ${SOURCES}
    ${HEADERS}
    ${SHADERS}
)

source_group("src" FILES ${SOURCES})
source_group("include" FILES ${HEADERS})
source_group("shaders" FILES ${SHADERS})

# 包含目录
target_include_directories(virtual_texturing PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/external/glfw/include
    ${CMAKE_SOURCE_DIR}/external/glad/include
    ${CMAKE_SOURCE_DIR}/external/glm
)

# 链接库
target_link_libraries(virtual_texturing PRIVATE
    OpenGL::GL
    glfw
)