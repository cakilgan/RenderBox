cmake_minimum_required(VERSION 3.10)

# Proje ismi ve sürümü
project(OpenGLTests VERSION 1.0)

# C++17 kullanmayı belirleyin
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# GLFW, GLAD ve GLM dizinleri
set(GLFW_DIR ${CMAKE_SOURCE_DIR}/glfw)
set(GLAD_DIR ${CMAKE_SOURCE_DIR}/glad)
set(GLM_DIR ${CMAKE_SOURCE_DIR}/glm)
set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/source)


# Kaynak dosyalar

# GLFW ve GLAD'ı alt dizinlerden ekle
add_subdirectory(glfw)
add_subdirectory(glad)
add_subdirectory(examples)
add_subdirectory(source)

add_subdirectory(glm)

add_library(core
    source/Application.cpp
    source/Camera.cpp
    source/Utils.cpp
    
)
target_include_directories(core PRIVATE
        ${GLM_DIR}
        ${GLFW_DIR}/include
        ${GLAD_DIR}
        ${SOURCE_DIR}
    )
target_link_libraries(core glfw glad glm)
target_include_directories(core PUBLIC ${CMAKE_SOURCE_DIR}/source)