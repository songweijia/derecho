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
CMAKE_SOURCE_DIR = /home/lorenzo/derecho

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/lorenzo/derecho/build

# Include any dependencies generated for this target.
include src/applications/archive/CMakeFiles/debug_failure_test.dir/depend.make

# Include the progress variables for this target.
include src/applications/archive/CMakeFiles/debug_failure_test.dir/progress.make

# Include the compile flags for this target's objects.
include src/applications/archive/CMakeFiles/debug_failure_test.dir/flags.make

src/applications/archive/CMakeFiles/debug_failure_test.dir/debug_failure_test.cpp.o: src/applications/archive/CMakeFiles/debug_failure_test.dir/flags.make
src/applications/archive/CMakeFiles/debug_failure_test.dir/debug_failure_test.cpp.o: ../src/applications/archive/debug_failure_test.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lorenzo/derecho/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/applications/archive/CMakeFiles/debug_failure_test.dir/debug_failure_test.cpp.o"
	cd /home/lorenzo/derecho/build/src/applications/archive && /usr/bin/g++-7  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/debug_failure_test.dir/debug_failure_test.cpp.o -c /home/lorenzo/derecho/src/applications/archive/debug_failure_test.cpp

src/applications/archive/CMakeFiles/debug_failure_test.dir/debug_failure_test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/debug_failure_test.dir/debug_failure_test.cpp.i"
	cd /home/lorenzo/derecho/build/src/applications/archive && /usr/bin/g++-7 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lorenzo/derecho/src/applications/archive/debug_failure_test.cpp > CMakeFiles/debug_failure_test.dir/debug_failure_test.cpp.i

src/applications/archive/CMakeFiles/debug_failure_test.dir/debug_failure_test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/debug_failure_test.dir/debug_failure_test.cpp.s"
	cd /home/lorenzo/derecho/build/src/applications/archive && /usr/bin/g++-7 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lorenzo/derecho/src/applications/archive/debug_failure_test.cpp -o CMakeFiles/debug_failure_test.dir/debug_failure_test.cpp.s

src/applications/archive/CMakeFiles/debug_failure_test.dir/debug_failure_test.cpp.o.requires:

.PHONY : src/applications/archive/CMakeFiles/debug_failure_test.dir/debug_failure_test.cpp.o.requires

src/applications/archive/CMakeFiles/debug_failure_test.dir/debug_failure_test.cpp.o.provides: src/applications/archive/CMakeFiles/debug_failure_test.dir/debug_failure_test.cpp.o.requires
	$(MAKE) -f src/applications/archive/CMakeFiles/debug_failure_test.dir/build.make src/applications/archive/CMakeFiles/debug_failure_test.dir/debug_failure_test.cpp.o.provides.build
.PHONY : src/applications/archive/CMakeFiles/debug_failure_test.dir/debug_failure_test.cpp.o.provides

src/applications/archive/CMakeFiles/debug_failure_test.dir/debug_failure_test.cpp.o.provides.build: src/applications/archive/CMakeFiles/debug_failure_test.dir/debug_failure_test.cpp.o


# Object files for target debug_failure_test
debug_failure_test_OBJECTS = \
"CMakeFiles/debug_failure_test.dir/debug_failure_test.cpp.o"

# External object files for target debug_failure_test
debug_failure_test_EXTERNAL_OBJECTS =

src/applications/archive/debug_failure_test: src/applications/archive/CMakeFiles/debug_failure_test.dir/debug_failure_test.cpp.o
src/applications/archive/debug_failure_test: src/applications/archive/CMakeFiles/debug_failure_test.dir/build.make
src/applications/archive/debug_failure_test: libderecho.so.0.9.1
src/applications/archive/debug_failure_test: /usr/local/lib/libfabric.so
src/applications/archive/debug_failure_test: src/applications/archive/CMakeFiles/debug_failure_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/lorenzo/derecho/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable debug_failure_test"
	cd /home/lorenzo/derecho/build/src/applications/archive && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/debug_failure_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/applications/archive/CMakeFiles/debug_failure_test.dir/build: src/applications/archive/debug_failure_test

.PHONY : src/applications/archive/CMakeFiles/debug_failure_test.dir/build

src/applications/archive/CMakeFiles/debug_failure_test.dir/requires: src/applications/archive/CMakeFiles/debug_failure_test.dir/debug_failure_test.cpp.o.requires

.PHONY : src/applications/archive/CMakeFiles/debug_failure_test.dir/requires

src/applications/archive/CMakeFiles/debug_failure_test.dir/clean:
	cd /home/lorenzo/derecho/build/src/applications/archive && $(CMAKE_COMMAND) -P CMakeFiles/debug_failure_test.dir/cmake_clean.cmake
.PHONY : src/applications/archive/CMakeFiles/debug_failure_test.dir/clean

src/applications/archive/CMakeFiles/debug_failure_test.dir/depend:
	cd /home/lorenzo/derecho/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/lorenzo/derecho /home/lorenzo/derecho/src/applications/archive /home/lorenzo/derecho/build /home/lorenzo/derecho/build/src/applications/archive /home/lorenzo/derecho/build/src/applications/archive/CMakeFiles/debug_failure_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/applications/archive/CMakeFiles/debug_failure_test.dir/depend

