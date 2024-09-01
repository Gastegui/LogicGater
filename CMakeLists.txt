cmake_minimum_required(VERSION 3.28)
project(LogicGater)

set(CMAKE_CXX_STANDARD 23)

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_image REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS} ${SDL2_MIXER_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})

add_executable(LogicGater main.cpp
        Wrapper/window.h
        Wrapper/sound.cpp
        Wrapper/sound.h
        Wrapper/text.h
        Wrapper/image.h
        puerta.cpp
        puerta.h
        Wrapper/image.cpp
        Wrapper/window.cpp
        raton.cpp
        raton.h
        controlador.cpp
        controlador.h
        io.cpp
        io.h
        entrada.cpp
        entrada.h
        salida.cpp
        salida.h)

target_link_libraries(LogicGater ${SDL2_LIBRARIES} SDL2_ttf SDL2_mixer SDL2_image)