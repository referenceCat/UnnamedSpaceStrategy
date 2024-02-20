include_directories(test PUBLIC game_engine)
include_directories(test PUBLIC game_engine/physics)
include_directories(test PUBLIC game_engine/ui)
add_executable(test main.cpp
        game_engine/ui/InputManager.h
        game_engine/ui/InputManager.cpp
        game_engine/GameEngine.h
        game_engine/GameEngine.cpp
        game_engine/physics/PhysicsEngine.h
        utils/Vector3d.h
        utils/Vector3d.cpp
        game_engine/physics/PhysicsEngine.cpp
        utils/OrbitParameters.h
        game_engine/ui/GraphicsEngine.h
        game_engine/ui/GraphicsEngine.cpp
        utils/Properties.cpp
        utils/Properties.h
)
find_package(PkgConfig REQUIRED)

pkg_check_modules(allegro-5 REQUIRED allegro-5 allegro_image-5 allegro_font-5 allegro_primitives-5 allegro_ttf-5)

include_directories(${PROJECT_SOURCE_DIR})

set(ALLEGRO5_LIBRARIES, "allegro_image,allegro_primitives;allegro")
message("PROJECT_SOURCE_DIR = ${PROJECT_SOURCE_DIR}")
message("ALLEGRO5_LIBRARIES = ${allegro-5_LIBRARIES}")
message("ALLEGRO5_INCLUDE_DIRS = ${allegro-5_INCLUDE_DIRS}")
target_link_libraries(test ${allegro-5_LIBRARIES})
target_include_directories(test PUBLIC ${allegro-5_INCLUDE_DIRS})