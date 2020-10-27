# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_SOURCE_DIR = /terraForce/UTD/cs6378/cs6378_project1

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /terraForce/UTD/cs6378/cs6378_project1/build

# Include any dependencies generated for this target.
include include/CMakeFiles/LibZ.dir/depend.make

# Include the progress variables for this target.
include include/CMakeFiles/LibZ.dir/progress.make

# Include the compile flags for this target's objects.
include include/CMakeFiles/LibZ.dir/flags.make

include/CMakeFiles/LibZ.dir/vecLib.o: include/CMakeFiles/LibZ.dir/flags.make
include/CMakeFiles/LibZ.dir/vecLib.o: ../include/vecLib.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/terraForce/UTD/cs6378/cs6378_project1/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object include/CMakeFiles/LibZ.dir/vecLib.o"
	cd /terraForce/UTD/cs6378/cs6378_project1/build/include && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/LibZ.dir/vecLib.o   -c /terraForce/UTD/cs6378/cs6378_project1/include/vecLib.c

include/CMakeFiles/LibZ.dir/vecLib.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/LibZ.dir/vecLib.i"
	cd /terraForce/UTD/cs6378/cs6378_project1/build/include && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /terraForce/UTD/cs6378/cs6378_project1/include/vecLib.c > CMakeFiles/LibZ.dir/vecLib.i

include/CMakeFiles/LibZ.dir/vecLib.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/LibZ.dir/vecLib.s"
	cd /terraForce/UTD/cs6378/cs6378_project1/build/include && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /terraForce/UTD/cs6378/cs6378_project1/include/vecLib.c -o CMakeFiles/LibZ.dir/vecLib.s

include/CMakeFiles/LibZ.dir/vecLib.o.requires:

.PHONY : include/CMakeFiles/LibZ.dir/vecLib.o.requires

include/CMakeFiles/LibZ.dir/vecLib.o.provides: include/CMakeFiles/LibZ.dir/vecLib.o.requires
	$(MAKE) -f include/CMakeFiles/LibZ.dir/build.make include/CMakeFiles/LibZ.dir/vecLib.o.provides.build
.PHONY : include/CMakeFiles/LibZ.dir/vecLib.o.provides

include/CMakeFiles/LibZ.dir/vecLib.o.provides.build: include/CMakeFiles/LibZ.dir/vecLib.o


# Object files for target LibZ
LibZ_OBJECTS = \
"CMakeFiles/LibZ.dir/vecLib.o"

# External object files for target LibZ
LibZ_EXTERNAL_OBJECTS =

include/libLibZ.a: include/CMakeFiles/LibZ.dir/vecLib.o
include/libLibZ.a: include/CMakeFiles/LibZ.dir/build.make
include/libLibZ.a: include/CMakeFiles/LibZ.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/terraForce/UTD/cs6378/cs6378_project1/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C static library libLibZ.a"
	cd /terraForce/UTD/cs6378/cs6378_project1/build/include && $(CMAKE_COMMAND) -P CMakeFiles/LibZ.dir/cmake_clean_target.cmake
	cd /terraForce/UTD/cs6378/cs6378_project1/build/include && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/LibZ.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
include/CMakeFiles/LibZ.dir/build: include/libLibZ.a

.PHONY : include/CMakeFiles/LibZ.dir/build

include/CMakeFiles/LibZ.dir/requires: include/CMakeFiles/LibZ.dir/vecLib.o.requires

.PHONY : include/CMakeFiles/LibZ.dir/requires

include/CMakeFiles/LibZ.dir/clean:
	cd /terraForce/UTD/cs6378/cs6378_project1/build/include && $(CMAKE_COMMAND) -P CMakeFiles/LibZ.dir/cmake_clean.cmake
.PHONY : include/CMakeFiles/LibZ.dir/clean

include/CMakeFiles/LibZ.dir/depend:
	cd /terraForce/UTD/cs6378/cs6378_project1/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /terraForce/UTD/cs6378/cs6378_project1 /terraForce/UTD/cs6378/cs6378_project1/include /terraForce/UTD/cs6378/cs6378_project1/build /terraForce/UTD/cs6378/cs6378_project1/build/include /terraForce/UTD/cs6378/cs6378_project1/build/include/CMakeFiles/LibZ.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : include/CMakeFiles/LibZ.dir/depend
