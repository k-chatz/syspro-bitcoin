# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

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
CMAKE_COMMAND = /opt/clion-2018.3.4/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /opt/clion-2018.3.4/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/msi/CLionProjects/bitcoin

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/msi/CLionProjects/bitcoin/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/bitcoin.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/bitcoin.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/bitcoin.dir/flags.make

CMakeFiles/bitcoin.dir/main.c.o: CMakeFiles/bitcoin.dir/flags.make
CMakeFiles/bitcoin.dir/main.c.o: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/msi/CLionProjects/bitcoin/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/bitcoin.dir/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/bitcoin.dir/main.c.o   -c /home/msi/CLionProjects/bitcoin/main.c

CMakeFiles/bitcoin.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/bitcoin.dir/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/msi/CLionProjects/bitcoin/main.c > CMakeFiles/bitcoin.dir/main.c.i

CMakeFiles/bitcoin.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/bitcoin.dir/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/msi/CLionProjects/bitcoin/main.c -o CMakeFiles/bitcoin.dir/main.c.s

CMakeFiles/bitcoin.dir/list.c.o: CMakeFiles/bitcoin.dir/flags.make
CMakeFiles/bitcoin.dir/list.c.o: ../list.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/msi/CLionProjects/bitcoin/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/bitcoin.dir/list.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/bitcoin.dir/list.c.o   -c /home/msi/CLionProjects/bitcoin/list.c

CMakeFiles/bitcoin.dir/list.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/bitcoin.dir/list.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/msi/CLionProjects/bitcoin/list.c > CMakeFiles/bitcoin.dir/list.c.i

CMakeFiles/bitcoin.dir/list.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/bitcoin.dir/list.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/msi/CLionProjects/bitcoin/list.c -o CMakeFiles/bitcoin.dir/list.c.s

CMakeFiles/bitcoin.dir/hash.c.o: CMakeFiles/bitcoin.dir/flags.make
CMakeFiles/bitcoin.dir/hash.c.o: ../hash.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/msi/CLionProjects/bitcoin/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/bitcoin.dir/hash.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/bitcoin.dir/hash.c.o   -c /home/msi/CLionProjects/bitcoin/hash.c

CMakeFiles/bitcoin.dir/hash.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/bitcoin.dir/hash.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/msi/CLionProjects/bitcoin/hash.c > CMakeFiles/bitcoin.dir/hash.c.i

CMakeFiles/bitcoin.dir/hash.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/bitcoin.dir/hash.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/msi/CLionProjects/bitcoin/hash.c -o CMakeFiles/bitcoin.dir/hash.c.s

# Object files for target bitcoin
bitcoin_OBJECTS = \
"CMakeFiles/bitcoin.dir/main.c.o" \
"CMakeFiles/bitcoin.dir/list.c.o" \
"CMakeFiles/bitcoin.dir/hash.c.o"

# External object files for target bitcoin
bitcoin_EXTERNAL_OBJECTS =

bitcoin: CMakeFiles/bitcoin.dir/main.c.o
bitcoin: CMakeFiles/bitcoin.dir/list.c.o
bitcoin: CMakeFiles/bitcoin.dir/hash.c.o
bitcoin: CMakeFiles/bitcoin.dir/build.make
bitcoin: CMakeFiles/bitcoin.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/msi/CLionProjects/bitcoin/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking C executable bitcoin"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/bitcoin.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/bitcoin.dir/build: bitcoin

.PHONY : CMakeFiles/bitcoin.dir/build

CMakeFiles/bitcoin.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/bitcoin.dir/cmake_clean.cmake
.PHONY : CMakeFiles/bitcoin.dir/clean

CMakeFiles/bitcoin.dir/depend:
	cd /home/msi/CLionProjects/bitcoin/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/msi/CLionProjects/bitcoin /home/msi/CLionProjects/bitcoin /home/msi/CLionProjects/bitcoin/cmake-build-debug /home/msi/CLionProjects/bitcoin/cmake-build-debug /home/msi/CLionProjects/bitcoin/cmake-build-debug/CMakeFiles/bitcoin.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/bitcoin.dir/depend

