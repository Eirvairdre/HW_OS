# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.31

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = B:\Cmake\bin\cmake.exe

# The command to remove a file.
RM = B:\Cmake\bin\cmake.exe -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\stoy2\CLionProjects\HW_4

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\stoy2\CLionProjects\HW_4\build

# Include any dependencies generated for this target.
include CMakeFiles/main.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/main.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/main.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/main.dir/flags.make

CMakeFiles/main.dir/codegen:
.PHONY : CMakeFiles/main.dir/codegen

CMakeFiles/main.dir/src/main.cpp.obj: CMakeFiles/main.dir/flags.make
CMakeFiles/main.dir/src/main.cpp.obj: CMakeFiles/main.dir/includes_CXX.rsp
CMakeFiles/main.dir/src/main.cpp.obj: C:/Users/stoy2/CLionProjects/HW_4/src/main.cpp
CMakeFiles/main.dir/src/main.cpp.obj: CMakeFiles/main.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=C:\Users\stoy2\CLionProjects\HW_4\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/main.dir/src/main.cpp.obj"
	B:\msys2\mingw64\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/main.dir/src/main.cpp.obj -MF CMakeFiles\main.dir\src\main.cpp.obj.d -o CMakeFiles\main.dir\src\main.cpp.obj -c C:\Users\stoy2\CLionProjects\HW_4\src\main.cpp

CMakeFiles/main.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/main.dir/src/main.cpp.i"
	B:\msys2\mingw64\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\stoy2\CLionProjects\HW_4\src\main.cpp > CMakeFiles\main.dir\src\main.cpp.i

CMakeFiles/main.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/main.dir/src/main.cpp.s"
	B:\msys2\mingw64\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\stoy2\CLionProjects\HW_4\src\main.cpp -o CMakeFiles\main.dir\src\main.cpp.s

CMakeFiles/main.dir/src/serial_port.cpp.obj: CMakeFiles/main.dir/flags.make
CMakeFiles/main.dir/src/serial_port.cpp.obj: CMakeFiles/main.dir/includes_CXX.rsp
CMakeFiles/main.dir/src/serial_port.cpp.obj: C:/Users/stoy2/CLionProjects/HW_4/src/serial_port.cpp
CMakeFiles/main.dir/src/serial_port.cpp.obj: CMakeFiles/main.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=C:\Users\stoy2\CLionProjects\HW_4\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/main.dir/src/serial_port.cpp.obj"
	B:\msys2\mingw64\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/main.dir/src/serial_port.cpp.obj -MF CMakeFiles\main.dir\src\serial_port.cpp.obj.d -o CMakeFiles\main.dir\src\serial_port.cpp.obj -c C:\Users\stoy2\CLionProjects\HW_4\src\serial_port.cpp

CMakeFiles/main.dir/src/serial_port.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/main.dir/src/serial_port.cpp.i"
	B:\msys2\mingw64\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\stoy2\CLionProjects\HW_4\src\serial_port.cpp > CMakeFiles\main.dir\src\serial_port.cpp.i

CMakeFiles/main.dir/src/serial_port.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/main.dir/src/serial_port.cpp.s"
	B:\msys2\mingw64\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\stoy2\CLionProjects\HW_4\src\serial_port.cpp -o CMakeFiles\main.dir\src\serial_port.cpp.s

CMakeFiles/main.dir/src/logger.cpp.obj: CMakeFiles/main.dir/flags.make
CMakeFiles/main.dir/src/logger.cpp.obj: CMakeFiles/main.dir/includes_CXX.rsp
CMakeFiles/main.dir/src/logger.cpp.obj: C:/Users/stoy2/CLionProjects/HW_4/src/logger.cpp
CMakeFiles/main.dir/src/logger.cpp.obj: CMakeFiles/main.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=C:\Users\stoy2\CLionProjects\HW_4\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/main.dir/src/logger.cpp.obj"
	B:\msys2\mingw64\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/main.dir/src/logger.cpp.obj -MF CMakeFiles\main.dir\src\logger.cpp.obj.d -o CMakeFiles\main.dir\src\logger.cpp.obj -c C:\Users\stoy2\CLionProjects\HW_4\src\logger.cpp

CMakeFiles/main.dir/src/logger.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/main.dir/src/logger.cpp.i"
	B:\msys2\mingw64\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\stoy2\CLionProjects\HW_4\src\logger.cpp > CMakeFiles\main.dir\src\logger.cpp.i

CMakeFiles/main.dir/src/logger.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/main.dir/src/logger.cpp.s"
	B:\msys2\mingw64\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\stoy2\CLionProjects\HW_4\src\logger.cpp -o CMakeFiles\main.dir\src\logger.cpp.s

CMakeFiles/main.dir/src/statistics.cpp.obj: CMakeFiles/main.dir/flags.make
CMakeFiles/main.dir/src/statistics.cpp.obj: CMakeFiles/main.dir/includes_CXX.rsp
CMakeFiles/main.dir/src/statistics.cpp.obj: C:/Users/stoy2/CLionProjects/HW_4/src/statistics.cpp
CMakeFiles/main.dir/src/statistics.cpp.obj: CMakeFiles/main.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=C:\Users\stoy2\CLionProjects\HW_4\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/main.dir/src/statistics.cpp.obj"
	B:\msys2\mingw64\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/main.dir/src/statistics.cpp.obj -MF CMakeFiles\main.dir\src\statistics.cpp.obj.d -o CMakeFiles\main.dir\src\statistics.cpp.obj -c C:\Users\stoy2\CLionProjects\HW_4\src\statistics.cpp

CMakeFiles/main.dir/src/statistics.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/main.dir/src/statistics.cpp.i"
	B:\msys2\mingw64\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\stoy2\CLionProjects\HW_4\src\statistics.cpp > CMakeFiles\main.dir\src\statistics.cpp.i

CMakeFiles/main.dir/src/statistics.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/main.dir/src/statistics.cpp.s"
	B:\msys2\mingw64\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\stoy2\CLionProjects\HW_4\src\statistics.cpp -o CMakeFiles\main.dir\src\statistics.cpp.s

CMakeFiles/main.dir/src/signal_handler.cpp.obj: CMakeFiles/main.dir/flags.make
CMakeFiles/main.dir/src/signal_handler.cpp.obj: CMakeFiles/main.dir/includes_CXX.rsp
CMakeFiles/main.dir/src/signal_handler.cpp.obj: C:/Users/stoy2/CLionProjects/HW_4/src/signal_handler.cpp
CMakeFiles/main.dir/src/signal_handler.cpp.obj: CMakeFiles/main.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=C:\Users\stoy2\CLionProjects\HW_4\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/main.dir/src/signal_handler.cpp.obj"
	B:\msys2\mingw64\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/main.dir/src/signal_handler.cpp.obj -MF CMakeFiles\main.dir\src\signal_handler.cpp.obj.d -o CMakeFiles\main.dir\src\signal_handler.cpp.obj -c C:\Users\stoy2\CLionProjects\HW_4\src\signal_handler.cpp

CMakeFiles/main.dir/src/signal_handler.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/main.dir/src/signal_handler.cpp.i"
	B:\msys2\mingw64\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\stoy2\CLionProjects\HW_4\src\signal_handler.cpp > CMakeFiles\main.dir\src\signal_handler.cpp.i

CMakeFiles/main.dir/src/signal_handler.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/main.dir/src/signal_handler.cpp.s"
	B:\msys2\mingw64\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\stoy2\CLionProjects\HW_4\src\signal_handler.cpp -o CMakeFiles\main.dir\src\signal_handler.cpp.s

# Object files for target main
main_OBJECTS = \
"CMakeFiles/main.dir/src/main.cpp.obj" \
"CMakeFiles/main.dir/src/serial_port.cpp.obj" \
"CMakeFiles/main.dir/src/logger.cpp.obj" \
"CMakeFiles/main.dir/src/statistics.cpp.obj" \
"CMakeFiles/main.dir/src/signal_handler.cpp.obj"

# External object files for target main
main_EXTERNAL_OBJECTS =

main.exe: CMakeFiles/main.dir/src/main.cpp.obj
main.exe: CMakeFiles/main.dir/src/serial_port.cpp.obj
main.exe: CMakeFiles/main.dir/src/logger.cpp.obj
main.exe: CMakeFiles/main.dir/src/statistics.cpp.obj
main.exe: CMakeFiles/main.dir/src/signal_handler.cpp.obj
main.exe: CMakeFiles/main.dir/build.make
main.exe: CMakeFiles/main.dir/linkLibs.rsp
main.exe: CMakeFiles/main.dir/objects1.rsp
main.exe: CMakeFiles/main.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=C:\Users\stoy2\CLionProjects\HW_4\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking CXX executable main.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\main.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/main.dir/build: main.exe
.PHONY : CMakeFiles/main.dir/build

CMakeFiles/main.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\main.dir\cmake_clean.cmake
.PHONY : CMakeFiles/main.dir/clean

CMakeFiles/main.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\stoy2\CLionProjects\HW_4 C:\Users\stoy2\CLionProjects\HW_4 C:\Users\stoy2\CLionProjects\HW_4\build C:\Users\stoy2\CLionProjects\HW_4\build C:\Users\stoy2\CLionProjects\HW_4\build\CMakeFiles\main.dir\DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/main.dir/depend

