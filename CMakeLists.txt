cmake_minimum_required(VERSION 3.11)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

project(AlpacaWeb)
set(CMAKE_CXX_STANDARD 17)

add_executable(AlpacaWeb main.cpp src/Window.cpp src/Window.h src/Game.cpp src/Game.h src/GameObjects/GameObject.cpp src/GameObjects/GameObject.h src/GameObjects/Cube.cpp src/GameObjects/Cube.h src/Materials/Material.cpp src/Materials/Material.h src/Materials/BasicMaterial.cpp src/Materials/BasicMaterial.h src/Models/Vertex.cpp src/Models/Vertex.h src/util/Utilities.cpp src/util/Utilities.h src/Materials/BasicTexturedMaterial.cpp src/Materials/BasicTexturedMaterial.h src/GameObjects/ModelObject.cpp src/GameObjects/ModelObject.h src/Materials/ShadowMaterial.cpp src/Materials/ShadowMaterial.h src/Renderer.cpp src/Renderer.h)

find_package(Vulkan REQUIRED)
target_include_directories(${PROJECT_NAME} PUBLIC ${Vulkan_INCLUDE_DIRS})
target_link_libraries(AlpacaWeb ${Vulkan_LIBRARIES})

add_subdirectory(glm)
target_link_libraries(AlpacaWeb glm)

add_subdirectory(glfw)
target_link_libraries(AlpacaWeb glfw)

include_directories(stb)
link_directories(AlpacaWeb stb)

include_directories(tinyobjloader)
link_directories(AlpacaWeb tinyobjloader)

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR}/out/)

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out CACHE STRING "" )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out CACHE STRING "" )