# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.15

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "D:\CLion 2019.2.3\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "D:\CLion 2019.2.3\bin\cmake\win\bin\cmake.exe" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = D:\home\algorithms

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = D:\home\algorithms\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/algorithms.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/algorithms.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/algorithms.dir/flags.make

CMakeFiles/algorithms.dir/tmp.cpp.obj: CMakeFiles/algorithms.dir/flags.make
CMakeFiles/algorithms.dir/tmp.cpp.obj: CMakeFiles/algorithms.dir/includes_CXX.rsp
CMakeFiles/algorithms.dir/tmp.cpp.obj: ../tmp.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\home\algorithms\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/algorithms.dir/tmp.cpp.obj"
	C:\PROGRA~2\MINGW-~1\I686-8~1.0-P\mingw32\bin\G__~1.EXE  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\algorithms.dir\tmp.cpp.obj -c D:\home\algorithms\tmp.cpp

CMakeFiles/algorithms.dir/tmp.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/algorithms.dir/tmp.cpp.i"
	C:\PROGRA~2\MINGW-~1\I686-8~1.0-P\mingw32\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\home\algorithms\tmp.cpp > CMakeFiles\algorithms.dir\tmp.cpp.i

CMakeFiles/algorithms.dir/tmp.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/algorithms.dir/tmp.cpp.s"
	C:\PROGRA~2\MINGW-~1\I686-8~1.0-P\mingw32\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\home\algorithms\tmp.cpp -o CMakeFiles\algorithms.dir\tmp.cpp.s

CMakeFiles/algorithms.dir/ttt.cpp.obj: CMakeFiles/algorithms.dir/flags.make
CMakeFiles/algorithms.dir/ttt.cpp.obj: CMakeFiles/algorithms.dir/includes_CXX.rsp
CMakeFiles/algorithms.dir/ttt.cpp.obj: ../ttt.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\home\algorithms\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/algorithms.dir/ttt.cpp.obj"
	C:\PROGRA~2\MINGW-~1\I686-8~1.0-P\mingw32\bin\G__~1.EXE  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\algorithms.dir\ttt.cpp.obj -c D:\home\algorithms\ttt.cpp

CMakeFiles/algorithms.dir/ttt.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/algorithms.dir/ttt.cpp.i"
	C:\PROGRA~2\MINGW-~1\I686-8~1.0-P\mingw32\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\home\algorithms\ttt.cpp > CMakeFiles\algorithms.dir\ttt.cpp.i

CMakeFiles/algorithms.dir/ttt.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/algorithms.dir/ttt.cpp.s"
	C:\PROGRA~2\MINGW-~1\I686-8~1.0-P\mingw32\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\home\algorithms\ttt.cpp -o CMakeFiles\algorithms.dir\ttt.cpp.s

# Object files for target algorithms
algorithms_OBJECTS = \
"CMakeFiles/algorithms.dir/tmp.cpp.obj" \
"CMakeFiles/algorithms.dir/ttt.cpp.obj"

# External object files for target algorithms
algorithms_EXTERNAL_OBJECTS =

algorithms.exe: CMakeFiles/algorithms.dir/tmp.cpp.obj
algorithms.exe: CMakeFiles/algorithms.dir/ttt.cpp.obj
algorithms.exe: CMakeFiles/algorithms.dir/build.make
algorithms.exe: CMakeFiles/algorithms.dir/linklibs.rsp
algorithms.exe: CMakeFiles/algorithms.dir/objects1.rsp
algorithms.exe: CMakeFiles/algorithms.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=D:\home\algorithms\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable algorithms.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\algorithms.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/algorithms.dir/build: algorithms.exe

.PHONY : CMakeFiles/algorithms.dir/build

CMakeFiles/algorithms.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\algorithms.dir\cmake_clean.cmake
.PHONY : CMakeFiles/algorithms.dir/clean

CMakeFiles/algorithms.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" D:\home\algorithms D:\home\algorithms D:\home\algorithms\cmake-build-debug D:\home\algorithms\cmake-build-debug D:\home\algorithms\cmake-build-debug\CMakeFiles\algorithms.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/algorithms.dir/depend
