# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.11

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
CMAKE_SOURCE_DIR = /home/alxhoff/git/GitHub/Datalog-API

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/alxhoff/git/GitHub/Datalog-API/build

# Include any dependencies generated for this target.
include CMakeFiles/EXAMPLE.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/EXAMPLE.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/EXAMPLE.dir/flags.make

CMakeFiles/EXAMPLE.dir/main.c.o: CMakeFiles/EXAMPLE.dir/flags.make
CMakeFiles/EXAMPLE.dir/main.c.o: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/alxhoff/git/GitHub/Datalog-API/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/EXAMPLE.dir/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/EXAMPLE.dir/main.c.o   -c /home/alxhoff/git/GitHub/Datalog-API/main.c

CMakeFiles/EXAMPLE.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/EXAMPLE.dir/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/alxhoff/git/GitHub/Datalog-API/main.c > CMakeFiles/EXAMPLE.dir/main.c.i

CMakeFiles/EXAMPLE.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/EXAMPLE.dir/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/alxhoff/git/GitHub/Datalog-API/main.c -o CMakeFiles/EXAMPLE.dir/main.c.s

# Object files for target EXAMPLE
EXAMPLE_OBJECTS = \
"CMakeFiles/EXAMPLE.dir/main.c.o"

# External object files for target EXAMPLE
EXAMPLE_EXTERNAL_OBJECTS =

../bin/EXAMPLE: CMakeFiles/EXAMPLE.dir/main.c.o
../bin/EXAMPLE: CMakeFiles/EXAMPLE.dir/build.make
../bin/EXAMPLE: libDATALOG_API.so
../bin/EXAMPLE: libDATALOG_PARSER.so
../bin/EXAMPLE: libDATALOG_OPCUA.so
../bin/EXAMPLE: libDATALOG.so
../bin/EXAMPLE: libLUA.so
../bin/EXAMPLE: ../libs/libxml2/lib/libxml2.so.2.9.7
../bin/EXAMPLE: CMakeFiles/EXAMPLE.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/alxhoff/git/GitHub/Datalog-API/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable ../bin/EXAMPLE"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/EXAMPLE.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/EXAMPLE.dir/build: ../bin/EXAMPLE

.PHONY : CMakeFiles/EXAMPLE.dir/build

CMakeFiles/EXAMPLE.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/EXAMPLE.dir/cmake_clean.cmake
.PHONY : CMakeFiles/EXAMPLE.dir/clean

CMakeFiles/EXAMPLE.dir/depend:
	cd /home/alxhoff/git/GitHub/Datalog-API/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/alxhoff/git/GitHub/Datalog-API /home/alxhoff/git/GitHub/Datalog-API /home/alxhoff/git/GitHub/Datalog-API/build /home/alxhoff/git/GitHub/Datalog-API/build /home/alxhoff/git/GitHub/Datalog-API/build/CMakeFiles/EXAMPLE.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/EXAMPLE.dir/depend

