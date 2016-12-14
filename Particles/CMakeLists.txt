file(GLOB SOURCES "./*.cpp" "./*.h")

add_library(Particles ${SOURCES})

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_gfx REQUIRED)
find_package(Box2D REQUIRED)

target_include_directories(Particles PUBLIC ./
	${BOX2D_INCLUDE_DIR}
	${SDL2_INCLUDE_DIR}
	${SDL2_IMAGE_INCLUDE_DIR}
	${SDL2_GFX_INCLUDE_DIR} )

target_link_libraries(Particles
	${BOX2D_LIBRARY}
	${SDL2_LIBRARY}
	${SDL2_IMAGE_LIBRARY}
	${SDL2_GFX_LIBRARY})

set(PARTICLES_HDRS
	Particles.h
	Particle.h
	ParticleSource.h
	LineParticleSource.h
	PointParticleSource.h
	ScatteredParticleSource.h
	RingParticleSource.h
	Vector.h )
	

install(TARGETS Particles DESTINATION lib)
install(FILES ${PARTICLES_HDRS} DESTINATION include)