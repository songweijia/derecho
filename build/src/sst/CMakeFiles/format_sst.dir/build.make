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

# Utility rule file for format_sst.

# Include the progress variables for this target.
include src/sst/CMakeFiles/format_sst.dir/progress.make

src/sst/CMakeFiles/format_sst:
	cd /home/lorenzo/derecho/build/src/sst && clang-format-3.8 -i *.cpp *.hpp

format_sst: src/sst/CMakeFiles/format_sst
format_sst: src/sst/CMakeFiles/format_sst.dir/build.make

.PHONY : format_sst

# Rule to build all files generated by this target.
src/sst/CMakeFiles/format_sst.dir/build: format_sst

.PHONY : src/sst/CMakeFiles/format_sst.dir/build

src/sst/CMakeFiles/format_sst.dir/clean:
	cd /home/lorenzo/derecho/build/src/sst && $(CMAKE_COMMAND) -P CMakeFiles/format_sst.dir/cmake_clean.cmake
.PHONY : src/sst/CMakeFiles/format_sst.dir/clean

src/sst/CMakeFiles/format_sst.dir/depend:
	cd /home/lorenzo/derecho/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/lorenzo/derecho /home/lorenzo/derecho/src/sst /home/lorenzo/derecho/build /home/lorenzo/derecho/build/src/sst /home/lorenzo/derecho/build/src/sst/CMakeFiles/format_sst.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/sst/CMakeFiles/format_sst.dir/depend

