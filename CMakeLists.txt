cmake_minimum_required(VERSION 2.8.12.2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
add_definitions("-std=c++11")


set (CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/install" CACHE PATH "default install path" FORCE )


project(SpaceParticles)

add_subdirectory(SpaceGame)
add_subdirectory(Particles)