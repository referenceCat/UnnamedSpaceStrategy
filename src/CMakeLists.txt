




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

target_include_directories(test PUBLIC game_engine)
target_include_directories(test PUBLIC game_engine/physics)
target_include_directories(test PUBLIC game_engine/ui)

target_link_libraries(test PUBLIC ${allegro-5_LIBRARIES})
target_include_directories(test PUBLIC ${allegro-5_INCLUDE_DIRS})


configure_file(game_engine/resources.h.in game_engine/resources.h @ONLY)
target_include_directories(test PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/game_engine)