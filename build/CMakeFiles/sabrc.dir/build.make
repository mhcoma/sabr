# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.19

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
CMAKE_COMMAND = "C:\Program Files\CMake\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\CMake\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Project\csabr

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Project\csabr\build

# Include any dependencies generated for this target.
include CMakeFiles/sabrc.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/sabrc.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/sabrc.dir/flags.make

CMakeFiles/sabrc.dir/src/compiler/compiler.obj: CMakeFiles/sabrc.dir/flags.make
CMakeFiles/sabrc.dir/src/compiler/compiler.obj: CMakeFiles/sabrc.dir/includes_C.rsp
CMakeFiles/sabrc.dir/src/compiler/compiler.obj: ../src/compiler/compiler.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Project\csabr\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/sabrc.dir/src/compiler/compiler.obj"
	C:\TDM-GCC-64\bin\x86_64-w64-mingw32-gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\sabrc.dir\src\compiler\compiler.obj -c C:\Project\csabr\src\compiler\compiler.c

CMakeFiles/sabrc.dir/src/compiler/compiler.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/sabrc.dir/src/compiler/compiler.i"
	C:\TDM-GCC-64\bin\x86_64-w64-mingw32-gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Project\csabr\src\compiler\compiler.c > CMakeFiles\sabrc.dir\src\compiler\compiler.i

CMakeFiles/sabrc.dir/src/compiler/compiler.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/sabrc.dir/src/compiler/compiler.s"
	C:\TDM-GCC-64\bin\x86_64-w64-mingw32-gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Project\csabr\src\compiler\compiler.c -o CMakeFiles\sabrc.dir\src\compiler\compiler.s

CMakeFiles/sabrc.dir/src/compiler/compiler_cctl_define.obj: CMakeFiles/sabrc.dir/flags.make
CMakeFiles/sabrc.dir/src/compiler/compiler_cctl_define.obj: CMakeFiles/sabrc.dir/includes_C.rsp
CMakeFiles/sabrc.dir/src/compiler/compiler_cctl_define.obj: ../src/compiler/compiler_cctl_define.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Project\csabr\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/sabrc.dir/src/compiler/compiler_cctl_define.obj"
	C:\TDM-GCC-64\bin\x86_64-w64-mingw32-gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\sabrc.dir\src\compiler\compiler_cctl_define.obj -c C:\Project\csabr\src\compiler\compiler_cctl_define.c

CMakeFiles/sabrc.dir/src/compiler/compiler_cctl_define.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/sabrc.dir/src/compiler/compiler_cctl_define.i"
	C:\TDM-GCC-64\bin\x86_64-w64-mingw32-gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Project\csabr\src\compiler\compiler_cctl_define.c > CMakeFiles\sabrc.dir\src\compiler\compiler_cctl_define.i

CMakeFiles/sabrc.dir/src/compiler/compiler_cctl_define.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/sabrc.dir/src/compiler/compiler_cctl_define.s"
	C:\TDM-GCC-64\bin\x86_64-w64-mingw32-gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Project\csabr\src\compiler\compiler_cctl_define.c -o CMakeFiles\sabrc.dir\src\compiler\compiler_cctl_define.s

CMakeFiles/sabrc.dir/src/compiler/compiler_main.obj: CMakeFiles/sabrc.dir/flags.make
CMakeFiles/sabrc.dir/src/compiler/compiler_main.obj: CMakeFiles/sabrc.dir/includes_C.rsp
CMakeFiles/sabrc.dir/src/compiler/compiler_main.obj: ../src/compiler/compiler_main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Project\csabr\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/sabrc.dir/src/compiler/compiler_main.obj"
	C:\TDM-GCC-64\bin\x86_64-w64-mingw32-gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\sabrc.dir\src\compiler\compiler_main.obj -c C:\Project\csabr\src\compiler\compiler_main.c

CMakeFiles/sabrc.dir/src/compiler/compiler_main.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/sabrc.dir/src/compiler/compiler_main.i"
	C:\TDM-GCC-64\bin\x86_64-w64-mingw32-gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Project\csabr\src\compiler\compiler_main.c > CMakeFiles\sabrc.dir\src\compiler\compiler_main.i

CMakeFiles/sabrc.dir/src/compiler/compiler_main.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/sabrc.dir/src/compiler/compiler_main.s"
	C:\TDM-GCC-64\bin\x86_64-w64-mingw32-gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Project\csabr\src\compiler\compiler_main.c -o CMakeFiles\sabrc.dir\src\compiler\compiler_main.s

CMakeFiles/sabrc.dir/src/compiler/contol.obj: CMakeFiles/sabrc.dir/flags.make
CMakeFiles/sabrc.dir/src/compiler/contol.obj: CMakeFiles/sabrc.dir/includes_C.rsp
CMakeFiles/sabrc.dir/src/compiler/contol.obj: ../src/compiler/contol.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Project\csabr\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/sabrc.dir/src/compiler/contol.obj"
	C:\TDM-GCC-64\bin\x86_64-w64-mingw32-gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\sabrc.dir\src\compiler\contol.obj -c C:\Project\csabr\src\compiler\contol.c

CMakeFiles/sabrc.dir/src/compiler/contol.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/sabrc.dir/src/compiler/contol.i"
	C:\TDM-GCC-64\bin\x86_64-w64-mingw32-gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Project\csabr\src\compiler\contol.c > CMakeFiles\sabrc.dir\src\compiler\contol.i

CMakeFiles/sabrc.dir/src/compiler/contol.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/sabrc.dir/src/compiler/contol.s"
	C:\TDM-GCC-64\bin\x86_64-w64-mingw32-gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Project\csabr\src\compiler\contol.c -o CMakeFiles\sabrc.dir\src\compiler\contol.s

CMakeFiles/sabrc.dir/src/compiler/operation.obj: CMakeFiles/sabrc.dir/flags.make
CMakeFiles/sabrc.dir/src/compiler/operation.obj: CMakeFiles/sabrc.dir/includes_C.rsp
CMakeFiles/sabrc.dir/src/compiler/operation.obj: ../src/compiler/operation.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Project\csabr\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object CMakeFiles/sabrc.dir/src/compiler/operation.obj"
	C:\TDM-GCC-64\bin\x86_64-w64-mingw32-gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\sabrc.dir\src\compiler\operation.obj -c C:\Project\csabr\src\compiler\operation.c

CMakeFiles/sabrc.dir/src/compiler/operation.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/sabrc.dir/src/compiler/operation.i"
	C:\TDM-GCC-64\bin\x86_64-w64-mingw32-gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Project\csabr\src\compiler\operation.c > CMakeFiles\sabrc.dir\src\compiler\operation.i

CMakeFiles/sabrc.dir/src/compiler/operation.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/sabrc.dir/src/compiler/operation.s"
	C:\TDM-GCC-64\bin\x86_64-w64-mingw32-gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Project\csabr\src\compiler\operation.c -o CMakeFiles\sabrc.dir\src\compiler\operation.s

CMakeFiles/sabrc.dir/src/rbt.obj: CMakeFiles/sabrc.dir/flags.make
CMakeFiles/sabrc.dir/src/rbt.obj: CMakeFiles/sabrc.dir/includes_C.rsp
CMakeFiles/sabrc.dir/src/rbt.obj: ../src/rbt.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Project\csabr\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building C object CMakeFiles/sabrc.dir/src/rbt.obj"
	C:\TDM-GCC-64\bin\x86_64-w64-mingw32-gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\sabrc.dir\src\rbt.obj -c C:\Project\csabr\src\rbt.c

CMakeFiles/sabrc.dir/src/rbt.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/sabrc.dir/src/rbt.i"
	C:\TDM-GCC-64\bin\x86_64-w64-mingw32-gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Project\csabr\src\rbt.c > CMakeFiles\sabrc.dir\src\rbt.i

CMakeFiles/sabrc.dir/src/rbt.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/sabrc.dir/src/rbt.s"
	C:\TDM-GCC-64\bin\x86_64-w64-mingw32-gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Project\csabr\src\rbt.c -o CMakeFiles\sabrc.dir\src\rbt.s

CMakeFiles/sabrc.dir/src/trie.obj: CMakeFiles/sabrc.dir/flags.make
CMakeFiles/sabrc.dir/src/trie.obj: CMakeFiles/sabrc.dir/includes_C.rsp
CMakeFiles/sabrc.dir/src/trie.obj: ../src/trie.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Project\csabr\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building C object CMakeFiles/sabrc.dir/src/trie.obj"
	C:\TDM-GCC-64\bin\x86_64-w64-mingw32-gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\sabrc.dir\src\trie.obj -c C:\Project\csabr\src\trie.c

CMakeFiles/sabrc.dir/src/trie.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/sabrc.dir/src/trie.i"
	C:\TDM-GCC-64\bin\x86_64-w64-mingw32-gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Project\csabr\src\trie.c > CMakeFiles\sabrc.dir\src\trie.i

CMakeFiles/sabrc.dir/src/trie.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/sabrc.dir/src/trie.s"
	C:\TDM-GCC-64\bin\x86_64-w64-mingw32-gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Project\csabr\src\trie.c -o CMakeFiles\sabrc.dir\src\trie.s

# Object files for target sabrc
sabrc_OBJECTS = \
"CMakeFiles/sabrc.dir/src/compiler/compiler.obj" \
"CMakeFiles/sabrc.dir/src/compiler/compiler_cctl_define.obj" \
"CMakeFiles/sabrc.dir/src/compiler/compiler_main.obj" \
"CMakeFiles/sabrc.dir/src/compiler/contol.obj" \
"CMakeFiles/sabrc.dir/src/compiler/operation.obj" \
"CMakeFiles/sabrc.dir/src/rbt.obj" \
"CMakeFiles/sabrc.dir/src/trie.obj"

# External object files for target sabrc
sabrc_EXTERNAL_OBJECTS =

sabrc.exe: CMakeFiles/sabrc.dir/src/compiler/compiler.obj
sabrc.exe: CMakeFiles/sabrc.dir/src/compiler/compiler_cctl_define.obj
sabrc.exe: CMakeFiles/sabrc.dir/src/compiler/compiler_main.obj
sabrc.exe: CMakeFiles/sabrc.dir/src/compiler/contol.obj
sabrc.exe: CMakeFiles/sabrc.dir/src/compiler/operation.obj
sabrc.exe: CMakeFiles/sabrc.dir/src/rbt.obj
sabrc.exe: CMakeFiles/sabrc.dir/src/trie.obj
sabrc.exe: CMakeFiles/sabrc.dir/build.make
sabrc.exe: CMakeFiles/sabrc.dir/linklibs.rsp
sabrc.exe: CMakeFiles/sabrc.dir/objects1.rsp
sabrc.exe: CMakeFiles/sabrc.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Project\csabr\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Linking C executable sabrc.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\sabrc.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/sabrc.dir/build: sabrc.exe

.PHONY : CMakeFiles/sabrc.dir/build

CMakeFiles/sabrc.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\sabrc.dir\cmake_clean.cmake
.PHONY : CMakeFiles/sabrc.dir/clean

CMakeFiles/sabrc.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Project\csabr C:\Project\csabr C:\Project\csabr\build C:\Project\csabr\build C:\Project\csabr\build\CMakeFiles\sabrc.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/sabrc.dir/depend

