cmake_minimum_required(VERSION 3.10)

# Global Settings
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)

# Binary
project(jin)

add_executable(${PROJECT_NAME}
  ${CMAKE_CURRENT_LIST_DIR}/src/driver.c
  ${CMAKE_CURRENT_LIST_DIR}/src/resource_manager/resm.c
  ${CMAKE_CURRENT_LIST_DIR}/src/sound/snd.c
)

target_link_libraries(${PROJECT_NAME} PUBLIC openal)

target_include_directories(${PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/src
)

target_link_directories(${PROJECT_NAME} PUBLIC
)

set_target_properties(${PROJECT_NAME} PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY bin
)
