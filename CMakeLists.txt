cmake_minimum_required(VERSION 2.8.12.2)
project(final_project)

file(GLOB SOURCES "src/*.cpp")
add_executable(final_project ${SOURCES})

# include particle system library
#add_library(Particles SHARED libs/src/*.cpp)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_gfx REQUIRED)
include_directories(include ${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR} ${SDL2_GFX_INCLUDE_DIR} libs/include)
target_link_libraries(final_project ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY} ${SDL2_GFX_LIBRARY})

