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
CMAKE_SOURCE_DIR = /home/alxhoff/git/Datalog-API

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/alxhoff/git/Datalog-API/build

# Include any dependencies generated for this target.
include CMakeFiles/DATALOG_API.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/DATALOG_API.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/DATALOG_API.dir/flags.make

CMakeFiles/DATALOG_API.dir/datalog_api.c.o: CMakeFiles/DATALOG_API.dir/flags.make
CMakeFiles/DATALOG_API.dir/datalog_api.c.o: ../datalog_api.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/alxhoff/git/Datalog-API/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/DATALOG_API.dir/datalog_api.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/DATALOG_API.dir/datalog_api.c.o   -c /home/alxhoff/git/Datalog-API/datalog_api.c

CMakeFiles/DATALOG_API.dir/datalog_api.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/DATALOG_API.dir/datalog_api.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/alxhoff/git/Datalog-API/datalog_api.c > CMakeFiles/DATALOG_API.dir/datalog_api.c.i

CMakeFiles/DATALOG_API.dir/datalog_api.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/DATALOG_API.dir/datalog_api.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/alxhoff/git/Datalog-API/datalog_api.c -o CMakeFiles/DATALOG_API.dir/datalog_api.c.s

CMakeFiles/DATALOG_API.dir/datalog_api.c.o.requires:

.PHONY : CMakeFiles/DATALOG_API.dir/datalog_api.c.o.requires

CMakeFiles/DATALOG_API.dir/datalog_api.c.o.provides: CMakeFiles/DATALOG_API.dir/datalog_api.c.o.requires
	$(MAKE) -f CMakeFiles/DATALOG_API.dir/build.make CMakeFiles/DATALOG_API.dir/datalog_api.c.o.provides.build
.PHONY : CMakeFiles/DATALOG_API.dir/datalog_api.c.o.provides

CMakeFiles/DATALOG_API.dir/datalog_api.c.o.provides.build: CMakeFiles/DATALOG_API.dir/datalog_api.c.o


CMakeFiles/DATALOG_API.dir/datalog_api_parser.c.o: CMakeFiles/DATALOG_API.dir/flags.make
CMakeFiles/DATALOG_API.dir/datalog_api_parser.c.o: ../datalog_api_parser.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/alxhoff/git/Datalog-API/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/DATALOG_API.dir/datalog_api_parser.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/DATALOG_API.dir/datalog_api_parser.c.o   -c /home/alxhoff/git/Datalog-API/datalog_api_parser.c

CMakeFiles/DATALOG_API.dir/datalog_api_parser.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/DATALOG_API.dir/datalog_api_parser.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/alxhoff/git/Datalog-API/datalog_api_parser.c > CMakeFiles/DATALOG_API.dir/datalog_api_parser.c.i

CMakeFiles/DATALOG_API.dir/datalog_api_parser.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/DATALOG_API.dir/datalog_api_parser.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/alxhoff/git/Datalog-API/datalog_api_parser.c -o CMakeFiles/DATALOG_API.dir/datalog_api_parser.c.s

CMakeFiles/DATALOG_API.dir/datalog_api_parser.c.o.requires:

.PHONY : CMakeFiles/DATALOG_API.dir/datalog_api_parser.c.o.requires

CMakeFiles/DATALOG_API.dir/datalog_api_parser.c.o.provides: CMakeFiles/DATALOG_API.dir/datalog_api_parser.c.o.requires
	$(MAKE) -f CMakeFiles/DATALOG_API.dir/build.make CMakeFiles/DATALOG_API.dir/datalog_api_parser.c.o.provides.build
.PHONY : CMakeFiles/DATALOG_API.dir/datalog_api_parser.c.o.provides

CMakeFiles/DATALOG_API.dir/datalog_api_parser.c.o.provides.build: CMakeFiles/DATALOG_API.dir/datalog_api_parser.c.o


CMakeFiles/DATALOG_API.dir/datalog_cli.c.o: CMakeFiles/DATALOG_API.dir/flags.make
CMakeFiles/DATALOG_API.dir/datalog_cli.c.o: ../datalog_cli.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/alxhoff/git/Datalog-API/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/DATALOG_API.dir/datalog_cli.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/DATALOG_API.dir/datalog_cli.c.o   -c /home/alxhoff/git/Datalog-API/datalog_cli.c

CMakeFiles/DATALOG_API.dir/datalog_cli.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/DATALOG_API.dir/datalog_cli.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/alxhoff/git/Datalog-API/datalog_cli.c > CMakeFiles/DATALOG_API.dir/datalog_cli.c.i

CMakeFiles/DATALOG_API.dir/datalog_cli.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/DATALOG_API.dir/datalog_cli.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/alxhoff/git/Datalog-API/datalog_cli.c -o CMakeFiles/DATALOG_API.dir/datalog_cli.c.s

CMakeFiles/DATALOG_API.dir/datalog_cli.c.o.requires:

.PHONY : CMakeFiles/DATALOG_API.dir/datalog_cli.c.o.requires

CMakeFiles/DATALOG_API.dir/datalog_cli.c.o.provides: CMakeFiles/DATALOG_API.dir/datalog_cli.c.o.requires
	$(MAKE) -f CMakeFiles/DATALOG_API.dir/build.make CMakeFiles/DATALOG_API.dir/datalog_cli.c.o.provides.build
.PHONY : CMakeFiles/DATALOG_API.dir/datalog_cli.c.o.provides

CMakeFiles/DATALOG_API.dir/datalog_cli.c.o.provides.build: CMakeFiles/DATALOG_API.dir/datalog_cli.c.o


CMakeFiles/DATALOG_API.dir/datalog_cli_asserter.c.o: CMakeFiles/DATALOG_API.dir/flags.make
CMakeFiles/DATALOG_API.dir/datalog_cli_asserter.c.o: ../datalog_cli_asserter.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/alxhoff/git/Datalog-API/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/DATALOG_API.dir/datalog_cli_asserter.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/DATALOG_API.dir/datalog_cli_asserter.c.o   -c /home/alxhoff/git/Datalog-API/datalog_cli_asserter.c

CMakeFiles/DATALOG_API.dir/datalog_cli_asserter.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/DATALOG_API.dir/datalog_cli_asserter.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/alxhoff/git/Datalog-API/datalog_cli_asserter.c > CMakeFiles/DATALOG_API.dir/datalog_cli_asserter.c.i

CMakeFiles/DATALOG_API.dir/datalog_cli_asserter.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/DATALOG_API.dir/datalog_cli_asserter.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/alxhoff/git/Datalog-API/datalog_cli_asserter.c -o CMakeFiles/DATALOG_API.dir/datalog_cli_asserter.c.s

CMakeFiles/DATALOG_API.dir/datalog_cli_asserter.c.o.requires:

.PHONY : CMakeFiles/DATALOG_API.dir/datalog_cli_asserter.c.o.requires

CMakeFiles/DATALOG_API.dir/datalog_cli_asserter.c.o.provides: CMakeFiles/DATALOG_API.dir/datalog_cli_asserter.c.o.requires
	$(MAKE) -f CMakeFiles/DATALOG_API.dir/build.make CMakeFiles/DATALOG_API.dir/datalog_cli_asserter.c.o.provides.build
.PHONY : CMakeFiles/DATALOG_API.dir/datalog_cli_asserter.c.o.provides

CMakeFiles/DATALOG_API.dir/datalog_cli_asserter.c.o.provides.build: CMakeFiles/DATALOG_API.dir/datalog_cli_asserter.c.o


# Object files for target DATALOG_API
DATALOG_API_OBJECTS = \
"CMakeFiles/DATALOG_API.dir/datalog_api.c.o" \
"CMakeFiles/DATALOG_API.dir/datalog_api_parser.c.o" \
"CMakeFiles/DATALOG_API.dir/datalog_cli.c.o" \
"CMakeFiles/DATALOG_API.dir/datalog_cli_asserter.c.o"

# External object files for target DATALOG_API
DATALOG_API_EXTERNAL_OBJECTS =

libDATALOG_API.so: CMakeFiles/DATALOG_API.dir/datalog_api.c.o
libDATALOG_API.so: CMakeFiles/DATALOG_API.dir/datalog_api_parser.c.o
libDATALOG_API.so: CMakeFiles/DATALOG_API.dir/datalog_cli.c.o
libDATALOG_API.so: CMakeFiles/DATALOG_API.dir/datalog_cli_asserter.c.o
libDATALOG_API.so: CMakeFiles/DATALOG_API.dir/build.make
libDATALOG_API.so: CMakeFiles/DATALOG_API.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/alxhoff/git/Datalog-API/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking C shared library libDATALOG_API.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/DATALOG_API.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/DATALOG_API.dir/build: libDATALOG_API.so

.PHONY : CMakeFiles/DATALOG_API.dir/build

CMakeFiles/DATALOG_API.dir/requires: CMakeFiles/DATALOG_API.dir/datalog_api.c.o.requires
CMakeFiles/DATALOG_API.dir/requires: CMakeFiles/DATALOG_API.dir/datalog_api_parser.c.o.requires
CMakeFiles/DATALOG_API.dir/requires: CMakeFiles/DATALOG_API.dir/datalog_cli.c.o.requires
CMakeFiles/DATALOG_API.dir/requires: CMakeFiles/DATALOG_API.dir/datalog_cli_asserter.c.o.requires

.PHONY : CMakeFiles/DATALOG_API.dir/requires

CMakeFiles/DATALOG_API.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/DATALOG_API.dir/cmake_clean.cmake
.PHONY : CMakeFiles/DATALOG_API.dir/clean

CMakeFiles/DATALOG_API.dir/depend:
	cd /home/alxhoff/git/Datalog-API/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/alxhoff/git/Datalog-API /home/alxhoff/git/Datalog-API /home/alxhoff/git/Datalog-API/build /home/alxhoff/git/Datalog-API/build /home/alxhoff/git/Datalog-API/build/CMakeFiles/DATALOG_API.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/DATALOG_API.dir/depend

