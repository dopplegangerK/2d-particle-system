file(GLOB LIB_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
add_library (Particles STATIC ${LIB_SOURCES})

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_gfx REQUIRED)

target_include_directories(Particles PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include ${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR} ${SDL2_GFX_INCLUDE_DIR})
target_link_libraries(Particles ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY} ${SDL2_GFX_LIBRARY})