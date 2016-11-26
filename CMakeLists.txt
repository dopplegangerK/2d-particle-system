cmake_minimum_required(VERSION 2.8.12.2)
project(FinalProject)

file(GLOB SOURCES "src/*.cpp")
file(GLOB BOX2D_SOURCES
	"box2d/*.cpp"
	"box2d/Collision/*.cpp"
	"box2d/Collision/Shapes/*.cpp"
	"box2d/Common/*"
	"box2d/Dynamics/*.cpp"
	"box2d/Dynamics/Contacts/*.cpp"
	"box2d/Dynamics/Joints/*.cpp"
	"box2d/Rope/*.cpp" )


add_executable (FinalProject ${SOURCES} ${BOX2D_SOURCES})

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_gfx REQUIRED)

target_include_directories(FinalProject PUBLIC include ./ ${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR} ${SDL2_GFX_INCLUDE_DIR} )
target_link_libraries(FinalProject ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY} ${SDL2_GFX_LIBRARY})