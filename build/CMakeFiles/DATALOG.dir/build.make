# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.9

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
CMAKE_SOURCE_DIR = /home/alxhoff/git/siemens/Datalog-API

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/alxhoff/git/siemens/Datalog-API/build

# Include any dependencies generated for this target.
include CMakeFiles/DATALOG.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/DATALOG.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/DATALOG.dir/flags.make

CMakeFiles/DATALOG.dir/libs/libdatalog/datalog.c.o: CMakeFiles/DATALOG.dir/flags.make
CMakeFiles/DATALOG.dir/libs/libdatalog/datalog.c.o: ../libs/libdatalog/datalog.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/alxhoff/git/siemens/Datalog-API/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/DATALOG.dir/libs/libdatalog/datalog.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/DATALOG.dir/libs/libdatalog/datalog.c.o   -c /home/alxhoff/git/siemens/Datalog-API/libs/libdatalog/datalog.c

CMakeFiles/DATALOG.dir/libs/libdatalog/datalog.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/DATALOG.dir/libs/libdatalog/datalog.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/alxhoff/git/siemens/Datalog-API/libs/libdatalog/datalog.c > CMakeFiles/DATALOG.dir/libs/libdatalog/datalog.c.i

CMakeFiles/DATALOG.dir/libs/libdatalog/datalog.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/DATALOG.dir/libs/libdatalog/datalog.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/alxhoff/git/siemens/Datalog-API/libs/libdatalog/datalog.c -o CMakeFiles/DATALOG.dir/libs/libdatalog/datalog.c.s

CMakeFiles/DATALOG.dir/libs/libdatalog/datalog.c.o.requires:

.PHONY : CMakeFiles/DATALOG.dir/libs/libdatalog/datalog.c.o.requires

CMakeFiles/DATALOG.dir/libs/libdatalog/datalog.c.o.provides: CMakeFiles/DATALOG.dir/libs/libdatalog/datalog.c.o.requires
	$(MAKE) -f CMakeFiles/DATALOG.dir/build.make CMakeFiles/DATALOG.dir/libs/libdatalog/datalog.c.o.provides.build
.PHONY : CMakeFiles/DATALOG.dir/libs/libdatalog/datalog.c.o.provides

CMakeFiles/DATALOG.dir/libs/libdatalog/datalog.c.o.provides.build: CMakeFiles/DATALOG.dir/libs/libdatalog/datalog.c.o


# Object files for target DATALOG
DATALOG_OBJECTS = \
"CMakeFiles/DATALOG.dir/libs/libdatalog/datalog.c.o"

# External object files for target DATALOG
DATALOG_EXTERNAL_OBJECTS =

libDATALOG.so: CMakeFiles/DATALOG.dir/libs/libdatalog/datalog.c.o
libDATALOG.so: CMakeFiles/DATALOG.dir/build.make
libDATALOG.so: CMakeFiles/DATALOG.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/alxhoff/git/siemens/Datalog-API/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C shared library libDATALOG.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/DATALOG.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/DATALOG.dir/build: libDATALOG.so

.PHONY : CMakeFiles/DATALOG.dir/build

CMakeFiles/DATALOG.dir/requires: CMakeFiles/DATALOG.dir/libs/libdatalog/datalog.c.o.requires

.PHONY : CMakeFiles/DATALOG.dir/requires

CMakeFiles/DATALOG.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/DATALOG.dir/cmake_clean.cmake
.PHONY : CMakeFiles/DATALOG.dir/clean

CMakeFiles/DATALOG.dir/depend:
	cd /home/alxhoff/git/siemens/Datalog-API/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/alxhoff/git/siemens/Datalog-API /home/alxhoff/git/siemens/Datalog-API /home/alxhoff/git/siemens/Datalog-API/build /home/alxhoff/git/siemens/Datalog-API/build /home/alxhoff/git/siemens/Datalog-API/build/CMakeFiles/DATALOG.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/DATALOG.dir/depend

