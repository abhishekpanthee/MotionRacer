cmake_minimum_required(VERSION 3.10)
project(MotionRacer)

set(CMAKE_CXX_STANDARD 17)

# Specify the path to SFML
set(SFML_DIR "/usr/local/share/cmake/SFML") #make sure this is setup very correclty 
# Adjust this path if necessary

find_package(SFML 2.6 REQUIRED graphics window system)

# Include directories
include_directories(${SFML_INCLUDE_DIR} ${CMAKE_SOURCE_DIR}/include)
link_directories(${SFML_LIBRARY_DIR})

add_executable(MotionRacer
    src/Car.cpp
    src/Game.cpp
    src/Main.cpp
    src/Obstacle.cpp
    src/PowerUp.cpp
    src/Menu.cpp
)

target_link_libraries(MotionRacer
    sfml-graphics
    sfml-window
    sfml-system
)

# Specify the path to the assets folder
set(ASSETS_DIR ${CMAKE_SOURCE_DIR}/assets)

# Add the post-build step to copy assets to the build directory
add_custom_command(TARGET MotionRacer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${ASSETS_DIR} $<TARGET_FILE_DIR:MotionRacer>/assets
)
