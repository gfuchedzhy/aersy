cmake_minimum_required(VERSION 3.3)
project(aersy CXX)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_EXTENSIONS off)

# search for glcxx
if(NOT GLCXX_ROOT AND DEFINED ENV{GLCXX_ROOT})
  set(GLCXX_ROOT $ENV{GLCXX_ROOT})
endif()
if(GLCXX_ROOT)
  find_path(GLCXX_INCLUDE_DIRS glcxx/renderer.hpp PATHS ${GLCXX_ROOT}/include NO_DEFAULT_PATH)
  find_library(GLCXX_LIBRARIES glcxx PATHS ${GLCXX_ROOT}/lib NO_DEFAULT_PATH)
endif()
find_path(GLCXX_INCLUDE_DIRS "glcxx/renderer.hpp")
find_library(GLCXX_LIBRARIES glcxx)
include(FindPackageHandleStandardArgs)
set(GLCXX_FIND_REQUIRED on)
find_package_handle_standard_args(GLCXX DEFAULT_MSG GLCXX_INCLUDE_DIRS GLCXX_LIBRARIES)

file(GLOB ENGINE_SRC src/engine/*.[hc]pp)
add_library(engine ${ENGINE_SRC})

find_package(SFML COMPONENTS window system graphics)
include_directories(src/engine ${SFML_INCLUDE_DIR} ${GLCXX_INCLUDE_DIRS})
add_definitions(-DAPPNAME="${PROJECT_NAME}")
file(GLOB APP_SRC src/app/*.[hc]pp)
add_executable(${PROJECT_NAME} ${APP_SRC})

file(GLOB SHADER_SRC src/engine/*.glsl)
include(glcxx_shaders)
glcxx_compile_shaders(shaders ${SHADER_SRC})

target_link_libraries(${PROJECT_NAME} engine shaders ${GLCXX_LIBRARIES} ${SFML_LIBRARIES} GL assimp)
