# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/nyz/MyminiSQL

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/nyz/MyminiSQL/build

# Include any dependencies generated for this target.
include CMakeFiles/MyminiSQL.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/MyminiSQL.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/MyminiSQL.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/MyminiSQL.dir/flags.make

CMakeFiles/MyminiSQL.dir/src/main.cpp.o: CMakeFiles/MyminiSQL.dir/flags.make
CMakeFiles/MyminiSQL.dir/src/main.cpp.o: ../src/main.cpp
CMakeFiles/MyminiSQL.dir/src/main.cpp.o: CMakeFiles/MyminiSQL.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nyz/MyminiSQL/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/MyminiSQL.dir/src/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/MyminiSQL.dir/src/main.cpp.o -MF CMakeFiles/MyminiSQL.dir/src/main.cpp.o.d -o CMakeFiles/MyminiSQL.dir/src/main.cpp.o -c /home/nyz/MyminiSQL/src/main.cpp

CMakeFiles/MyminiSQL.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MyminiSQL.dir/src/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/nyz/MyminiSQL/src/main.cpp > CMakeFiles/MyminiSQL.dir/src/main.cpp.i

CMakeFiles/MyminiSQL.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MyminiSQL.dir/src/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/nyz/MyminiSQL/src/main.cpp -o CMakeFiles/MyminiSQL.dir/src/main.cpp.s

# Object files for target MyminiSQL
MyminiSQL_OBJECTS = \
"CMakeFiles/MyminiSQL.dir/src/main.cpp.o"

# External object files for target MyminiSQL
MyminiSQL_EXTERNAL_OBJECTS =

../bin/MyminiSQL: CMakeFiles/MyminiSQL.dir/src/main.cpp.o
../bin/MyminiSQL: CMakeFiles/MyminiSQL.dir/build.make
../bin/MyminiSQL: CMakeFiles/MyminiSQL.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/nyz/MyminiSQL/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../bin/MyminiSQL"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/MyminiSQL.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/MyminiSQL.dir/build: ../bin/MyminiSQL
.PHONY : CMakeFiles/MyminiSQL.dir/build

CMakeFiles/MyminiSQL.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/MyminiSQL.dir/cmake_clean.cmake
.PHONY : CMakeFiles/MyminiSQL.dir/clean

CMakeFiles/MyminiSQL.dir/depend:
	cd /home/nyz/MyminiSQL/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/nyz/MyminiSQL /home/nyz/MyminiSQL /home/nyz/MyminiSQL/build /home/nyz/MyminiSQL/build /home/nyz/MyminiSQL/build/CMakeFiles/MyminiSQL.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/MyminiSQL.dir/depend
