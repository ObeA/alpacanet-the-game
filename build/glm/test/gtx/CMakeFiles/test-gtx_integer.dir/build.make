# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.14

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/obe/dev/projects/school/graphics/alpacanet-the-game

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/obe/dev/projects/school/graphics/alpacanet-the-game/build

# Include any dependencies generated for this target.
include glm/test/gtx/CMakeFiles/test-gtx_integer.dir/depend.make

# Include the progress variables for this target.
include glm/test/gtx/CMakeFiles/test-gtx_integer.dir/progress.make

# Include the compile flags for this target's objects.
include glm/test/gtx/CMakeFiles/test-gtx_integer.dir/flags.make

glm/test/gtx/CMakeFiles/test-gtx_integer.dir/gtx_integer.cpp.o: glm/test/gtx/CMakeFiles/test-gtx_integer.dir/flags.make
glm/test/gtx/CMakeFiles/test-gtx_integer.dir/gtx_integer.cpp.o: ../glm/test/gtx/gtx_integer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/obe/dev/projects/school/graphics/alpacanet-the-game/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object glm/test/gtx/CMakeFiles/test-gtx_integer.dir/gtx_integer.cpp.o"
	cd /home/obe/dev/projects/school/graphics/alpacanet-the-game/build/glm/test/gtx && /usr/bin/clang++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test-gtx_integer.dir/gtx_integer.cpp.o -c /home/obe/dev/projects/school/graphics/alpacanet-the-game/glm/test/gtx/gtx_integer.cpp

glm/test/gtx/CMakeFiles/test-gtx_integer.dir/gtx_integer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test-gtx_integer.dir/gtx_integer.cpp.i"
	cd /home/obe/dev/projects/school/graphics/alpacanet-the-game/build/glm/test/gtx && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/obe/dev/projects/school/graphics/alpacanet-the-game/glm/test/gtx/gtx_integer.cpp > CMakeFiles/test-gtx_integer.dir/gtx_integer.cpp.i

glm/test/gtx/CMakeFiles/test-gtx_integer.dir/gtx_integer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test-gtx_integer.dir/gtx_integer.cpp.s"
	cd /home/obe/dev/projects/school/graphics/alpacanet-the-game/build/glm/test/gtx && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/obe/dev/projects/school/graphics/alpacanet-the-game/glm/test/gtx/gtx_integer.cpp -o CMakeFiles/test-gtx_integer.dir/gtx_integer.cpp.s

# Object files for target test-gtx_integer
test__gtx_integer_OBJECTS = \
"CMakeFiles/test-gtx_integer.dir/gtx_integer.cpp.o"

# External object files for target test-gtx_integer
test__gtx_integer_EXTERNAL_OBJECTS =

out/test-gtx_integer: glm/test/gtx/CMakeFiles/test-gtx_integer.dir/gtx_integer.cpp.o
out/test-gtx_integer: glm/test/gtx/CMakeFiles/test-gtx_integer.dir/build.make
out/test-gtx_integer: glm/test/gtx/CMakeFiles/test-gtx_integer.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/obe/dev/projects/school/graphics/alpacanet-the-game/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../../../out/test-gtx_integer"
	cd /home/obe/dev/projects/school/graphics/alpacanet-the-game/build/glm/test/gtx && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test-gtx_integer.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
glm/test/gtx/CMakeFiles/test-gtx_integer.dir/build: out/test-gtx_integer

.PHONY : glm/test/gtx/CMakeFiles/test-gtx_integer.dir/build

glm/test/gtx/CMakeFiles/test-gtx_integer.dir/clean:
	cd /home/obe/dev/projects/school/graphics/alpacanet-the-game/build/glm/test/gtx && $(CMAKE_COMMAND) -P CMakeFiles/test-gtx_integer.dir/cmake_clean.cmake
.PHONY : glm/test/gtx/CMakeFiles/test-gtx_integer.dir/clean

glm/test/gtx/CMakeFiles/test-gtx_integer.dir/depend:
	cd /home/obe/dev/projects/school/graphics/alpacanet-the-game/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/obe/dev/projects/school/graphics/alpacanet-the-game /home/obe/dev/projects/school/graphics/alpacanet-the-game/glm/test/gtx /home/obe/dev/projects/school/graphics/alpacanet-the-game/build /home/obe/dev/projects/school/graphics/alpacanet-the-game/build/glm/test/gtx /home/obe/dev/projects/school/graphics/alpacanet-the-game/build/glm/test/gtx/CMakeFiles/test-gtx_integer.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : glm/test/gtx/CMakeFiles/test-gtx_integer.dir/depend

