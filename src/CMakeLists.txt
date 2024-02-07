include_directories(test PUBLIC game_engine)
include_directories(test PUBLIC orbital_mechanics)
include_directories(test PUBLIC ui)
add_executable(test main.cpp
        ui/InputManager.h
        ui/InputManager.cpp)
find_package(PkgConfig REQUIRED)

include_directories(${PROJECT_SOURCE_DIR})

pkg_check_modules(ALLEGRO5 REQUIRED allegro-5)
target_link_libraries(test ${ALLEGRO5_LIBRARIES})
target_include_directories(test PUBLIC ${ALLEGRO5_INCLUDE_DIRS})