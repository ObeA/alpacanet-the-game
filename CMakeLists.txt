cmake_minimum_required(VERSION 3.13)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

project(AlpacaWeb)

message(STATUS "Using module to find Vulkan")
find_package(Vulkan)
message(STATUS ${Vulkan_LIBRARY})

set(CMAKE_CXX_STANDARD 17)

add_executable(vulkaninfo vulkaninfo.c)
target_compile_definitions(vulkaninfo PRIVATE VK_USE_PLATFORM_WIN32_KHR)

add_executable(AlpacaWeb main.cpp)

add_subdirectory(glfw)
include_directories(glfw/include)

target_link_libraries(AlpacaWeb vulkan ${Vulkan_LIBRARY})
target_link_libraries(AlpacaWeb glfw ${GLFW_LIBRARIES})