cmake_minimum_required(VERSION 3.13)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

project(AlpacaWeb)
set(CMAKE_CXX_STANDARD 17)

add_executable(AlpacaWeb main.cpp)

find_package(Vulkan)
target_link_libraries(AlpacaWeb ${Vulkan_LIBRARY})

add_subdirectory(glfw)
target_link_libraries(AlpacaWeb glfw)