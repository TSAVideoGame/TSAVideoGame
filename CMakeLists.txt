cmake_minimum_required(VERSION 3.10)

# Global Settings
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)
set(WINDOWS_LIBRARIES_DIRECTORIES "C:/cpplib")

# Binary
project(jin)

add_executable(${PROJECT_NAME}
  ${CMAKE_CURRENT_LIST_DIR}/src/driver.c
  ${CMAKE_CURRENT_LIST_DIR}/src/anim/anim.c
  ${CMAKE_CURRENT_LIST_DIR}/src/core/core.c
  ${CMAKE_CURRENT_LIST_DIR}/src/core/globals.c
  ${CMAKE_CURRENT_LIST_DIR}/src/core/env/env.c
  ${CMAKE_CURRENT_LIST_DIR}/src/core/gll/gll.c
  ${CMAKE_CURRENT_LIST_DIR}/src/core/input/input.c
  ${CMAKE_CURRENT_LIST_DIR}/src/core/logger/logger.c
  ${CMAKE_CURRENT_LIST_DIR}/src/core/thread/thread.c
  ${CMAKE_CURRENT_LIST_DIR}/src/core/window/window.c
  ${CMAKE_CURRENT_LIST_DIR}/src/gfx/gfx.c
  ${CMAKE_CURRENT_LIST_DIR}/src/resm/resm.c
  ${CMAKE_CURRENT_LIST_DIR}/src/snd/snd.c
  ${CMAKE_CURRENT_LIST_DIR}/src/snd/wav_util.c
  ${CMAKE_CURRENT_LIST_DIR}/src/stm/stm.c
  ${CMAKE_CURRENT_LIST_DIR}/src/stm/states.c

  ${CMAKE_CURRENT_LIST_DIR}/libs/lodepng/lodepng.c
)

target_include_directories(${PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/src
  ${CMAKE_CURRENT_LIST_DIR}/libs
  ${WINDOWS_LIBRARIES_DIRECTORIES}
)

set_target_properties(${PROJECT_NAME} PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY bin
)

# Libraries
set(CORE_LIBS "")

if (UNIX)
  set(CORE_LIBS "-lX11 -lGL -lpthread -lm")
endif (UNIX)

# Assuming WIN32 is MSVC, need to fix that
if (WIN32)
  set(CORE_LIBS "opengl32")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /Zc:preprocessor")
endif (WIN32)

target_link_libraries(${PROJECT_NAME} PUBLIC ${CORE_LIBS} openal jel)

