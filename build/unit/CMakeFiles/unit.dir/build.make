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
include unit/CMakeFiles/unit.dir/depend.make

# Include the progress variables for this target.
include unit/CMakeFiles/unit.dir/progress.make

# Include the compile flags for this target's objects.
include unit/CMakeFiles/unit.dir/flags.make

unit/CMakeFiles/unit.dir/unit.o: unit/CMakeFiles/unit.dir/flags.make
unit/CMakeFiles/unit.dir/unit.o: ../unit/unit.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/terraForce/UTD/cs6378/cs6378_project1/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object unit/CMakeFiles/unit.dir/unit.o"
	cd /terraForce/UTD/cs6378/cs6378_project1/build/unit && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/unit.dir/unit.o   -c /terraForce/UTD/cs6378/cs6378_project1/unit/unit.c

unit/CMakeFiles/unit.dir/unit.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/unit.dir/unit.i"
	cd /terraForce/UTD/cs6378/cs6378_project1/build/unit && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /terraForce/UTD/cs6378/cs6378_project1/unit/unit.c > CMakeFiles/unit.dir/unit.i

unit/CMakeFiles/unit.dir/unit.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/unit.dir/unit.s"
	cd /terraForce/UTD/cs6378/cs6378_project1/build/unit && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /terraForce/UTD/cs6378/cs6378_project1/unit/unit.c -o CMakeFiles/unit.dir/unit.s

unit/CMakeFiles/unit.dir/unit.o.requires:

.PHONY : unit/CMakeFiles/unit.dir/unit.o.requires

unit/CMakeFiles/unit.dir/unit.o.provides: unit/CMakeFiles/unit.dir/unit.o.requires
	$(MAKE) -f unit/CMakeFiles/unit.dir/build.make unit/CMakeFiles/unit.dir/unit.o.provides.build
.PHONY : unit/CMakeFiles/unit.dir/unit.o.provides

unit/CMakeFiles/unit.dir/unit.o.provides.build: unit/CMakeFiles/unit.dir/unit.o


# Object files for target unit
unit_OBJECTS = \
"CMakeFiles/unit.dir/unit.o"

# External object files for target unit
unit_EXTERNAL_OBJECTS =

unit/unit: unit/CMakeFiles/unit.dir/unit.o
unit/unit: unit/CMakeFiles/unit.dir/build.make
unit/unit: include/libLibZ.a
unit/unit: unit/CMakeFiles/unit.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/terraForce/UTD/cs6378/cs6378_project1/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable unit"
	cd /terraForce/UTD/cs6378/cs6378_project1/build/unit && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/unit.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
unit/CMakeFiles/unit.dir/build: unit/unit

.PHONY : unit/CMakeFiles/unit.dir/build

unit/CMakeFiles/unit.dir/requires: unit/CMakeFiles/unit.dir/unit.o.requires

.PHONY : unit/CMakeFiles/unit.dir/requires

unit/CMakeFiles/unit.dir/clean:
	cd /terraForce/UTD/cs6378/cs6378_project1/build/unit && $(CMAKE_COMMAND) -P CMakeFiles/unit.dir/cmake_clean.cmake
.PHONY : unit/CMakeFiles/unit.dir/clean

unit/CMakeFiles/unit.dir/depend:
	cd /terraForce/UTD/cs6378/cs6378_project1/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /terraForce/UTD/cs6378/cs6378_project1 /terraForce/UTD/cs6378/cs6378_project1/unit /terraForce/UTD/cs6378/cs6378_project1/build /terraForce/UTD/cs6378/cs6378_project1/build/unit /terraForce/UTD/cs6378/cs6378_project1/build/unit/CMakeFiles/unit.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : unit/CMakeFiles/unit.dir/depend

