file(GLOB SOURCES "./*.cpp" "./*.h")

add_library(Particles ${SOURCES})

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_gfx REQUIRED)

target_include_directories(Particles PUBLIC ./ ../ ${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR} ${SDL2_GFX_INCLUDE_DIR} )

find_library(BOX2D_LIBRARY
	NAMES Box2D
	HINTS ../)
target_link_libraries(Particles ${BOX2D_LIBRARY} ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY} ${SDL2_GFX_LIBRARY})