cmake_minimum_required(VERSION 3.11)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

project(AlpacaWeb)
set(CMAKE_CXX_STANDARD 17)

add_executable(AlpacaWeb main.cpp src/Window.cpp src/Window.h src/Game.cpp src/Game.h)

find_package(Vulkan REQUIRED)
target_include_directories(${PROJECT_NAME} PUBLIC ${Vulkan_INCLUDE_DIRS})
target_link_libraries(AlpacaWeb ${Vulkan_LIBRARIES})

add_subdirectory(glm)
target_link_libraries(AlpacaWeb glm)

add_subdirectory(glfw)
target_link_libraries(AlpacaWeb glfw)


set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out CACHE STRING "" )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out CACHE STRING "" )