file(GLOB SOURCES "src/*.cpp")
add_executable (FinalProject ${SOURCES})

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_gfx REQUIRED)

link_directories(%{CMAKE_CURRENT_SOURCE_DIR}/build/particles/Debug)
target_include_directories(FinalProject PUBLIC include ${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR} ${SDL2_GFX_INCLUDE_DIR} )
target_link_libraries(FinalProject Particles ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY} ${SDL2_GFX_LIBRARY})