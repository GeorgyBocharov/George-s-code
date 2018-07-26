cmake_minimum_required(VERSION 3.2)
project(Game)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp Controllable.h dlc.h Metheors.h Enemy.h ControlFunctions.h
        EngineFunctions.h Bullets.h Lifebar.h Lifebar_h.h Menu_h.h Menu.h Bonus.h)

add_executable(Game ${SOURCE_FILES})

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake-build-debug")
find_package(SFML REQUIRED system window graphics network audio)
if (SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(Game ${SFML_LIBRARIES})
endif()
