cmake_minimum_required(VERSION 3.11)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

project(AlpacaWeb)
set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE AlpacaWeb_SRC
        "src/*.h"
        "src/*.cpp"
)
add_executable(AlpacaWeb ${AlpacaWeb_SRC})

find_package(Vulkan REQUIRED)
target_include_directories(${PROJECT_NAME} PUBLIC ${Vulkan_INCLUDE_DIRS})
target_link_libraries(AlpacaWeb ${Vulkan_LIBRARIES})

add_subdirectory(glm)
target_link_libraries(AlpacaWeb glm)

add_subdirectory(glfw)
# find_package(glfw3 REQUIRED)
target_link_libraries(AlpacaWeb glfw)

include_directories(stb)
link_directories(AlpacaWeb stb)

include_directories(tinyobjloader)
link_directories(AlpacaWeb tinyobjloader)

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR}/out/)

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out CACHE STRING "" )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out CACHE STRING "" )