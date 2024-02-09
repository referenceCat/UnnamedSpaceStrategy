include_directories(test PUBLIC game_engine)
include_directories(test PUBLIC game_engine/physics)
include_directories(test PUBLIC game_engine/ui)
add_executable(test main.cpp
        game_engine/ui/InputManager.h
        game_engine/ui/InputManager.cpp
        game_engine/GameEngine.h
        game_engine/GameEngine.cpp
        game_engine/physics/PhysicsEngine.h
        game_engine/physics/Vector3d.h
        game_engine/physics/Vector3d.cpp
)
find_package(PkgConfig REQUIRED)

include_directories(${PROJECT_SOURCE_DIR})

pkg_check_modules(ALLEGRO5 REQUIRED allegro-5)
target_link_libraries(test ${ALLEGRO5_LIBRARIES})
target_include_directories(test PUBLIC ${ALLEGRO5_INCLUDE_DIRS})