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
include glm/test/ext/CMakeFiles/test-ext_scalar_common.dir/depend.make

# Include the progress variables for this target.
include glm/test/ext/CMakeFiles/test-ext_scalar_common.dir/progress.make

# Include the compile flags for this target's objects.
include glm/test/ext/CMakeFiles/test-ext_scalar_common.dir/flags.make

glm/test/ext/CMakeFiles/test-ext_scalar_common.dir/ext_scalar_common.cpp.o: glm/test/ext/CMakeFiles/test-ext_scalar_common.dir/flags.make
glm/test/ext/CMakeFiles/test-ext_scalar_common.dir/ext_scalar_common.cpp.o: ../glm/test/ext/ext_scalar_common.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/obe/dev/projects/school/graphics/alpacanet-the-game/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object glm/test/ext/CMakeFiles/test-ext_scalar_common.dir/ext_scalar_common.cpp.o"
	cd /home/obe/dev/projects/school/graphics/alpacanet-the-game/build/glm/test/ext && /usr/bin/clang++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test-ext_scalar_common.dir/ext_scalar_common.cpp.o -c /home/obe/dev/projects/school/graphics/alpacanet-the-game/glm/test/ext/ext_scalar_common.cpp

glm/test/ext/CMakeFiles/test-ext_scalar_common.dir/ext_scalar_common.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test-ext_scalar_common.dir/ext_scalar_common.cpp.i"
	cd /home/obe/dev/projects/school/graphics/alpacanet-the-game/build/glm/test/ext && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/obe/dev/projects/school/graphics/alpacanet-the-game/glm/test/ext/ext_scalar_common.cpp > CMakeFiles/test-ext_scalar_common.dir/ext_scalar_common.cpp.i

glm/test/ext/CMakeFiles/test-ext_scalar_common.dir/ext_scalar_common.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test-ext_scalar_common.dir/ext_scalar_common.cpp.s"
	cd /home/obe/dev/projects/school/graphics/alpacanet-the-game/build/glm/test/ext && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/obe/dev/projects/school/graphics/alpacanet-the-game/glm/test/ext/ext_scalar_common.cpp -o CMakeFiles/test-ext_scalar_common.dir/ext_scalar_common.cpp.s

# Object files for target test-ext_scalar_common
test__ext_scalar_common_OBJECTS = \
"CMakeFiles/test-ext_scalar_common.dir/ext_scalar_common.cpp.o"

# External object files for target test-ext_scalar_common
test__ext_scalar_common_EXTERNAL_OBJECTS =

out/test-ext_scalar_common: glm/test/ext/CMakeFiles/test-ext_scalar_common.dir/ext_scalar_common.cpp.o
out/test-ext_scalar_common: glm/test/ext/CMakeFiles/test-ext_scalar_common.dir/build.make
out/test-ext_scalar_common: glm/test/ext/CMakeFiles/test-ext_scalar_common.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/obe/dev/projects/school/graphics/alpacanet-the-game/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../../../out/test-ext_scalar_common"
	cd /home/obe/dev/projects/school/graphics/alpacanet-the-game/build/glm/test/ext && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test-ext_scalar_common.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
glm/test/ext/CMakeFiles/test-ext_scalar_common.dir/build: out/test-ext_scalar_common

.PHONY : glm/test/ext/CMakeFiles/test-ext_scalar_common.dir/build

glm/test/ext/CMakeFiles/test-ext_scalar_common.dir/clean:
	cd /home/obe/dev/projects/school/graphics/alpacanet-the-game/build/glm/test/ext && $(CMAKE_COMMAND) -P CMakeFiles/test-ext_scalar_common.dir/cmake_clean.cmake
.PHONY : glm/test/ext/CMakeFiles/test-ext_scalar_common.dir/clean

glm/test/ext/CMakeFiles/test-ext_scalar_common.dir/depend:
	cd /home/obe/dev/projects/school/graphics/alpacanet-the-game/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/obe/dev/projects/school/graphics/alpacanet-the-game /home/obe/dev/projects/school/graphics/alpacanet-the-game/glm/test/ext /home/obe/dev/projects/school/graphics/alpacanet-the-game/build /home/obe/dev/projects/school/graphics/alpacanet-the-game/build/glm/test/ext /home/obe/dev/projects/school/graphics/alpacanet-the-game/build/glm/test/ext/CMakeFiles/test-ext_scalar_common.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : glm/test/ext/CMakeFiles/test-ext_scalar_common.dir/depend

