cmake_minimum_required(VERSION 3.16)
project(Music-Visualizer VERSION 1.0)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)


if(WIN32)
  set(CMAKE_REQUIRED_FLAGS "-lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lm")
else()
  set(CMAKE_REQUIRED_FLAGS "-lSDL2 -lSDL2_image -lm")
endif()

find_library(SDL2_LIBRARY NAMES SDL2)
find_library(SDL2_LIBRARY_MAIN NAMES SDL2main)
find_library(SDL2_IMAGE_LIBRARY NAMES SDL2_image)

message("")
if(NOT SDL2_LIBRARY_MAIN)
  message(WARNING "SDL2main not found!")
else()
  message(STATUS "SDL2main found at ${SDL2_LIBRARY_MAIN}")
endif()

if(NOT SDL2_LIBRARY)
  message(FATAL_ERROR "SDL2 not found, please install it via your package manager!")
else()
  message(STATUS "SDL2 found at ${SDL2_LIBRARY}")
endif()

if(NOT SDL2_IMAGE_LIBRARY)
  message(FATAL_ERROR "SDL2_image not found, please install it via your package manager!")
else()
  message(STATUS "SDL2_image found at ${SDL2_IMAGE_LIBRARY}")
endif()

message("")

file(GLOB SOURCES "src/*.c")

if(WIN32)
  add_executable(PengerMans WIN32 ${SOURCES})
else()
  add_executable(PengerMans ${SOURCES})
endif()

if(CMAKE_C_COMPILER_ID STREQUAL "GNU" OR CMAKE_C_COMPILER_ID STREQUAL "Clang")
  target_compile_options(PengerMans PRIVATE -Wall -Wextra -pedantic)
endif()

if(WIN32)
  target_compile_definitions(PengerMans PUBLIC SDL_MAIN_HANDLED)
  target_link_libraries(PengerMans ${SDL2_LIBRARY_MAIN} ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY} m)
else()
  target_link_libraries(PengerMans ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY} m)
endif()



