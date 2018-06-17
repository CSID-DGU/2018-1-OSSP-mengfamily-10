cmake_minimum_required(VERSION 3.9)
project(2017_2_OSSP_Dongguk_Banjeom_4_master)

set(CMAKE_CXX_STANDARD 11)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})


add_executable(2017_2_OSSP_Dongguk_Banjeom_4_master
        audio.c
        audio.h
        config.h
        frame.c
        shapes.c
        tetris.c
        tetris.h
<<<<<<< HEAD
        util.c client.c)
=======
        util.c
        buffer.h audio.c)
>>>>>>> master

target_link_libraries(2017_2_OSSP_Dongguk_Banjeom_4_master ${SDL2_LIBRARIES})