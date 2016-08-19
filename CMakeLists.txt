cmake_minimum_required(VERSION 3.3)
project(aersy CXX)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_EXTENSIONS off)

add_subdirectory(glcxx)
include_directories(${GLCXX_INCLUDE_DIRS})

file(GLOB ENGINE_SRC src/engine/*.[hc]pp)
add_library(engine ${ENGINE_SRC})

find_package(SFML COMPONENTS window system graphics)
include_directories(src/engine ${SFML_INCLUDE_DIR})

include_directories(src/engine)
add_definitions(-DAPPNAME="${PROJECT_NAME}")
file(GLOB APP_SRC src/app/*.[hc]pp)
add_executable(${PROJECT_NAME} ${APP_SRC})

file(GLOB SHADER_SRC src/engine/*.glsl)
include(glcxx_shaders)
glcxx_compile_shaders(shaders ${SHADER_SRC})

target_link_libraries(${PROJECT_NAME} engine shaders ${GLCXX_LIBRARIES} ${SFML_LIBRARIES} GL assimp)
