file(GLOB SOURCES "src/*")

add_executable (Demo ${SOURCES})

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_gfx REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_mixer REQUIRED)

target_include_directories(Demo PUBLIC ./ ../
	${SDL2_INCLUDE_DIR}
	${SDL2_IMAGE_INCLUDE_DIR}
	${SDL2_GFX_INCLUDE_DIR}
	${SDL2_TTF_INCLUDE_DIR}
	${SDL2_MIXER_INCLUDE_DIR})

find_library(BOX2D_LIBRARY
	NAMES Box2D
	HINTS ../)
target_link_libraries(Demo PUBLIC Particles
	${BOX2D_LIBRARY}
	${SDL2_LIBRARY}
	${SDL2_IMAGE_LIBRARY}
	${SDL2_GFX_LIBRARY}
	${SDL2_TTF_LIBRARY}
	${SDL2_MIXER_LIBRARY})