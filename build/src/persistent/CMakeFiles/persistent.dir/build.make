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
include src/persistent/CMakeFiles/persistent.dir/depend.make

# Include the progress variables for this target.
include src/persistent/CMakeFiles/persistent.dir/progress.make

# Include the compile flags for this target's objects.
include src/persistent/CMakeFiles/persistent.dir/flags.make

src/persistent/CMakeFiles/persistent.dir/Persistent.cpp.o: src/persistent/CMakeFiles/persistent.dir/flags.make
src/persistent/CMakeFiles/persistent.dir/Persistent.cpp.o: ../src/persistent/Persistent.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lorenzo/derecho/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/persistent/CMakeFiles/persistent.dir/Persistent.cpp.o"
	cd /home/lorenzo/derecho/build/src/persistent && /usr/bin/g++-7  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/persistent.dir/Persistent.cpp.o -c /home/lorenzo/derecho/src/persistent/Persistent.cpp

src/persistent/CMakeFiles/persistent.dir/Persistent.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/persistent.dir/Persistent.cpp.i"
	cd /home/lorenzo/derecho/build/src/persistent && /usr/bin/g++-7 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lorenzo/derecho/src/persistent/Persistent.cpp > CMakeFiles/persistent.dir/Persistent.cpp.i

src/persistent/CMakeFiles/persistent.dir/Persistent.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/persistent.dir/Persistent.cpp.s"
	cd /home/lorenzo/derecho/build/src/persistent && /usr/bin/g++-7 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lorenzo/derecho/src/persistent/Persistent.cpp -o CMakeFiles/persistent.dir/Persistent.cpp.s

src/persistent/CMakeFiles/persistent.dir/Persistent.cpp.o.requires:

.PHONY : src/persistent/CMakeFiles/persistent.dir/Persistent.cpp.o.requires

src/persistent/CMakeFiles/persistent.dir/Persistent.cpp.o.provides: src/persistent/CMakeFiles/persistent.dir/Persistent.cpp.o.requires
	$(MAKE) -f src/persistent/CMakeFiles/persistent.dir/build.make src/persistent/CMakeFiles/persistent.dir/Persistent.cpp.o.provides.build
.PHONY : src/persistent/CMakeFiles/persistent.dir/Persistent.cpp.o.provides

src/persistent/CMakeFiles/persistent.dir/Persistent.cpp.o.provides.build: src/persistent/CMakeFiles/persistent.dir/Persistent.cpp.o


src/persistent/CMakeFiles/persistent.dir/PersistLog.cpp.o: src/persistent/CMakeFiles/persistent.dir/flags.make
src/persistent/CMakeFiles/persistent.dir/PersistLog.cpp.o: ../src/persistent/PersistLog.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lorenzo/derecho/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/persistent/CMakeFiles/persistent.dir/PersistLog.cpp.o"
	cd /home/lorenzo/derecho/build/src/persistent && /usr/bin/g++-7  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/persistent.dir/PersistLog.cpp.o -c /home/lorenzo/derecho/src/persistent/PersistLog.cpp

src/persistent/CMakeFiles/persistent.dir/PersistLog.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/persistent.dir/PersistLog.cpp.i"
	cd /home/lorenzo/derecho/build/src/persistent && /usr/bin/g++-7 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lorenzo/derecho/src/persistent/PersistLog.cpp > CMakeFiles/persistent.dir/PersistLog.cpp.i

src/persistent/CMakeFiles/persistent.dir/PersistLog.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/persistent.dir/PersistLog.cpp.s"
	cd /home/lorenzo/derecho/build/src/persistent && /usr/bin/g++-7 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lorenzo/derecho/src/persistent/PersistLog.cpp -o CMakeFiles/persistent.dir/PersistLog.cpp.s

src/persistent/CMakeFiles/persistent.dir/PersistLog.cpp.o.requires:

.PHONY : src/persistent/CMakeFiles/persistent.dir/PersistLog.cpp.o.requires

src/persistent/CMakeFiles/persistent.dir/PersistLog.cpp.o.provides: src/persistent/CMakeFiles/persistent.dir/PersistLog.cpp.o.requires
	$(MAKE) -f src/persistent/CMakeFiles/persistent.dir/build.make src/persistent/CMakeFiles/persistent.dir/PersistLog.cpp.o.provides.build
.PHONY : src/persistent/CMakeFiles/persistent.dir/PersistLog.cpp.o.provides

src/persistent/CMakeFiles/persistent.dir/PersistLog.cpp.o.provides.build: src/persistent/CMakeFiles/persistent.dir/PersistLog.cpp.o


src/persistent/CMakeFiles/persistent.dir/FilePersistLog.cpp.o: src/persistent/CMakeFiles/persistent.dir/flags.make
src/persistent/CMakeFiles/persistent.dir/FilePersistLog.cpp.o: ../src/persistent/FilePersistLog.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lorenzo/derecho/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object src/persistent/CMakeFiles/persistent.dir/FilePersistLog.cpp.o"
	cd /home/lorenzo/derecho/build/src/persistent && /usr/bin/g++-7  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/persistent.dir/FilePersistLog.cpp.o -c /home/lorenzo/derecho/src/persistent/FilePersistLog.cpp

src/persistent/CMakeFiles/persistent.dir/FilePersistLog.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/persistent.dir/FilePersistLog.cpp.i"
	cd /home/lorenzo/derecho/build/src/persistent && /usr/bin/g++-7 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lorenzo/derecho/src/persistent/FilePersistLog.cpp > CMakeFiles/persistent.dir/FilePersistLog.cpp.i

src/persistent/CMakeFiles/persistent.dir/FilePersistLog.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/persistent.dir/FilePersistLog.cpp.s"
	cd /home/lorenzo/derecho/build/src/persistent && /usr/bin/g++-7 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lorenzo/derecho/src/persistent/FilePersistLog.cpp -o CMakeFiles/persistent.dir/FilePersistLog.cpp.s

src/persistent/CMakeFiles/persistent.dir/FilePersistLog.cpp.o.requires:

.PHONY : src/persistent/CMakeFiles/persistent.dir/FilePersistLog.cpp.o.requires

src/persistent/CMakeFiles/persistent.dir/FilePersistLog.cpp.o.provides: src/persistent/CMakeFiles/persistent.dir/FilePersistLog.cpp.o.requires
	$(MAKE) -f src/persistent/CMakeFiles/persistent.dir/build.make src/persistent/CMakeFiles/persistent.dir/FilePersistLog.cpp.o.provides.build
.PHONY : src/persistent/CMakeFiles/persistent.dir/FilePersistLog.cpp.o.provides

src/persistent/CMakeFiles/persistent.dir/FilePersistLog.cpp.o.provides.build: src/persistent/CMakeFiles/persistent.dir/FilePersistLog.cpp.o


src/persistent/CMakeFiles/persistent.dir/HLC.cpp.o: src/persistent/CMakeFiles/persistent.dir/flags.make
src/persistent/CMakeFiles/persistent.dir/HLC.cpp.o: ../src/persistent/HLC.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lorenzo/derecho/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object src/persistent/CMakeFiles/persistent.dir/HLC.cpp.o"
	cd /home/lorenzo/derecho/build/src/persistent && /usr/bin/g++-7  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/persistent.dir/HLC.cpp.o -c /home/lorenzo/derecho/src/persistent/HLC.cpp

src/persistent/CMakeFiles/persistent.dir/HLC.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/persistent.dir/HLC.cpp.i"
	cd /home/lorenzo/derecho/build/src/persistent && /usr/bin/g++-7 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lorenzo/derecho/src/persistent/HLC.cpp > CMakeFiles/persistent.dir/HLC.cpp.i

src/persistent/CMakeFiles/persistent.dir/HLC.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/persistent.dir/HLC.cpp.s"
	cd /home/lorenzo/derecho/build/src/persistent && /usr/bin/g++-7 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lorenzo/derecho/src/persistent/HLC.cpp -o CMakeFiles/persistent.dir/HLC.cpp.s

src/persistent/CMakeFiles/persistent.dir/HLC.cpp.o.requires:

.PHONY : src/persistent/CMakeFiles/persistent.dir/HLC.cpp.o.requires

src/persistent/CMakeFiles/persistent.dir/HLC.cpp.o.provides: src/persistent/CMakeFiles/persistent.dir/HLC.cpp.o.requires
	$(MAKE) -f src/persistent/CMakeFiles/persistent.dir/build.make src/persistent/CMakeFiles/persistent.dir/HLC.cpp.o.provides.build
.PHONY : src/persistent/CMakeFiles/persistent.dir/HLC.cpp.o.provides

src/persistent/CMakeFiles/persistent.dir/HLC.cpp.o.provides.build: src/persistent/CMakeFiles/persistent.dir/HLC.cpp.o


persistent: src/persistent/CMakeFiles/persistent.dir/Persistent.cpp.o
persistent: src/persistent/CMakeFiles/persistent.dir/PersistLog.cpp.o
persistent: src/persistent/CMakeFiles/persistent.dir/FilePersistLog.cpp.o
persistent: src/persistent/CMakeFiles/persistent.dir/HLC.cpp.o
persistent: src/persistent/CMakeFiles/persistent.dir/build.make

.PHONY : persistent

# Rule to build all files generated by this target.
src/persistent/CMakeFiles/persistent.dir/build: persistent

.PHONY : src/persistent/CMakeFiles/persistent.dir/build

src/persistent/CMakeFiles/persistent.dir/requires: src/persistent/CMakeFiles/persistent.dir/Persistent.cpp.o.requires
src/persistent/CMakeFiles/persistent.dir/requires: src/persistent/CMakeFiles/persistent.dir/PersistLog.cpp.o.requires
src/persistent/CMakeFiles/persistent.dir/requires: src/persistent/CMakeFiles/persistent.dir/FilePersistLog.cpp.o.requires
src/persistent/CMakeFiles/persistent.dir/requires: src/persistent/CMakeFiles/persistent.dir/HLC.cpp.o.requires

.PHONY : src/persistent/CMakeFiles/persistent.dir/requires

src/persistent/CMakeFiles/persistent.dir/clean:
	cd /home/lorenzo/derecho/build/src/persistent && $(CMAKE_COMMAND) -P CMakeFiles/persistent.dir/cmake_clean.cmake
.PHONY : src/persistent/CMakeFiles/persistent.dir/clean

src/persistent/CMakeFiles/persistent.dir/depend:
	cd /home/lorenzo/derecho/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/lorenzo/derecho /home/lorenzo/derecho/src/persistent /home/lorenzo/derecho/build /home/lorenzo/derecho/build/src/persistent /home/lorenzo/derecho/build/src/persistent/CMakeFiles/persistent.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/persistent/CMakeFiles/persistent.dir/depend

