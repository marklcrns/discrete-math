################################################################################
# Generic Makefile for c++ with single src executable and multiple unit test
# executables.
#
# Support for profiling tools: valgrind, gprof, gcov, lcov
#
# Author:	Mark Lucernas
# Date:		2021-04-11
################################################################################
# Ideal project structure:
#
# root/
# 	|-Makefile (this)
# 	|-include  (optional)
# 	|-src/
# 		|-main.cpp
# 		|-submodule/
# 			|-...
# 		|-submodule/
# 		|-...
# 	|-test/
# 		|-test_main.cpp
# 		|-...cpp
# 		|-submodule/
# 			|-...
# 		|-submodule/
# 		|-.../
#
# Makefile generated files below...
#
# 	|-build/
# 		|-src/
# 			|-main.o
# 			|-submodule/
# 				|-...
# 			|-submodule/
# 			|-.../
# 		|-test/
# 			|-test_main.o
# 			|-...o
# 			|-submodule/
# 				|-...
# 			|-submodule/
# 			|-.../
# 	|-bin/
# 		|-main
# 		|-test_main
# 		|-...
#
# NOTE: This Makefile needs to be placed in the project root directory.
################################################################################
# Resources:
# 	https://stackoverflow.com/a/2484343
# 	https://stackoverflow.com/a/13696012
# 	https://github.com/awdeorio/supermakefilecxx
################################################################################

CC := g++
LD := $(CC)

# Compiler flags
# -std=c++14         C++ 14 standard
# -g3                Level 3 debug information
# -pedantic          Strict ISO C++
# -Wall              Enable many warning flags
# -Wextra            Enable more warning flags
# -Wconversion       Warn for implicit conversions that may alter a value
# -Wsign-conversion  Warn for implicit conversions that may change the sign of
#                    an integer value
# -Werror            Treat all warnings as errors
CFLAGS 		?= -std=c++14 \
		-g3 -ggdb3 \
		-Wpedantic -Wall -Wextra -Warray-bounds \
		-Weffc++ -Wconversion -Wsign-conversion \
		# -Werror
LDFLAGS		?=

SRC_DIR		:= src
BUILD_DIR	:= build
TEST_DIR	:= test
PROF_DIR	:= prof
BIN_DIR		:= bin

INCLUDE		:= include
TARGET 		:= pos_int_algo_sols
TARGET_EXT	:=
RUN_TARGET_ARGS	:=

# '.' indicates the src or test directory itself
MODULES   	:= . mutils
TEST_MODULES 	:= .

# Use mingw-w64 64-bit or 32-bit compiler
MINGW_W64	:= 1
MINGW_W32	:= 0

# Other tools
GCOV		?= gcov
LCOV		?= lcov
GPROF		?= gprof
VALGRIND 	?= valgrind
GCOV_FLAGS	?= --relative-only
LCOV_FLAGS	?= --capture
GPROF_FLAGS	?=
VALGRIND_FLAGS	?= --track-origins=yes --tool=memcheck \
		--leak-check=full --show-leak-kinds=all

# Enable profiling tools necessary flags
E_GCOV		:= 0
E_GPROF		:= 0

#==============================================================================#
################## DOES NOT NEED CHANGING BELOW THIS LINE ######################
#==============================================================================#

ifeq ($(MINGW_W64), 1)
CC 		:= x86_64-w64-mingw32-g++
LD 		:= $(CC)
LDFLAGS		+= -static-libgcc -static-libstdc++
TARGET_EXT	:= .exe
endif

ifeq ($(MINGW_W32), 1)
CC 		:= i686-w64-mingw32-g++
LD 		:= $(CC)
LDFLAGS		+= -static-libgcc -static-libstdc++
TARGET_EXT	:= .exe
endif

ifeq ($(E_GCOV), 1)
CFLAGS		+= -fprofile-arcs -ftest-coverage
LDFLAGS		+= -lgcov --coverage
endif

ifeq ($(E_GPROF), 1)
CFLAGS		+= -pg
LDFLAGS		+= -pg
endif

CXX 		:= $(CC)
CXXFLAGS 	:= $(CFLAGS)

SRC_MODULES   	:= $(addprefix $(SRC_DIR)/,$(MODULES))
SRC_BUILD_DIR 	:= $(addprefix $(BUILD_DIR)/$(SRC_DIR)/,$(MODULES))
SRC       	:= $(foreach sdir,$(SRC_MODULES),$(wildcard $(sdir)/*.cpp))
OBJ       	:= $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/$(SRC_DIR)/%.o,$(SRC))

TEST_SRC_DIR		:= $(addprefix 	$(TEST_DIR)/,$(TEST_MODULES))
TEST_BUILD_DIR 	:= $(addprefix $(BUILD_DIR)/$(TEST_DIR)/,$(TEST_MODULES))
TEST_SRC 	:= $(foreach tdir,$(TEST_SRC_DIR),$(wildcard $(tdir)/*.cpp))
TEST_OBJ  	:= $(patsubst $(TEST_DIR)/%.cpp,$(BUILD_DIR)/$(TEST_DIR)/%.o,$(TEST_SRC))
TEST_TARGETS	:= $(patsubst $(TEST_DIR)/%.cpp,$(BIN_DIR)/%$(TARGET_EXT),$(TEST_SRC))
# Objects with target excluded (workaround for multiple 'main' definition error)
OBJ_TARGET_EXCL	:= $(filter-out $(BUILD_DIR)/$(SRC_DIR)/./$(TARGET).o,$(OBJ))

INC		:= $(addprefix -I,$(SRC_MODULES)) $(addprefix -I,$(INCLUDE))

vpath %.cpp $(SRC_MODULES) $(TEST_SRC_DIR)

define make-target-objs
$1/%.o: %.cpp
	$(CC) $(CFLAGS) $(INC) -c $$< -o $$@
endef


default: checkdirs $(TARGET)$(TARGET_EXT)

test: checkdirs $(TEST_TARGETS)

all: checkdirs $(TARGET)$(TARGET_EXT) $(TEST_TARGETS)

checkdirs: $(SRC_BUILD_DIR) $(TEST_BUILD_DIR) $(BIN_DIR) $(PROF_DIR)

new:
	make clean
	make all

.PHONY: default test all checkdirs new

# ==================== Help ==================== #

help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... default (the default if no target is provided)"
	@echo "... test"
	@echo "... all"
	@echo "... new"
	@echo "... checkdirs"
	@echo "... run"
	@echo "... runtest"
	@echo "... memcheck     (against main)"
	@echo "... memchecktest (against test suites)"
	@echo "... gprof        (against main. requires E_GPROF=1)"
	@echo "... gcov         (against main. requires E_GCOV=1)"
	@echo "... lcov         (against main. requires E_GCOV=1)"
	@echo "... clean-prof   (clean all profiler outputs)"
	@echo "... clean-out    (clear all objects and executables)"
	@echo "... clean        (runs clean-prof and clean-out)"
	@echo "... cleanest     (clean all)"
	@echo "... help"
	@echo "..."
	@echo "... E_GPROF=1    enable gprof compiler flags"
	@echo "... E_GCOV=1     enable gcov compiler flags"
	@echo "... MINGW_W64=1  to use mingw-w64 for 64-bit compiler"
	@echo "... MINGW_W32=1  to use mingw-w64 for 32-bit compiler"

.PHONY : help

# ==================== SRC FILES ==================== #

# Target Files
$(TARGET)$(TARGET_EXT): $(OBJ)
	$(LD) $(LDFLAGS) $^ -o $(BIN_DIR)/$@

# Object files (make-objs)
$(foreach bdir,$(SRC_BUILD_DIR),$(eval $(call make-target-objs,$(bdir))))

run: default
	./$(BIN_DIR)/$(TARGET)$(TARGET_EXT) $(RUN_TARGET_ARGS)

memcheck: default
	$(VALGRIND) $(VALGRIND_FLAGS) ./$(BIN_DIR)/$(TARGET)$(TARGET_EXT) $(RUN_TARGET_ARGS)

.PHONY: run memcheck

# ==================== TEST FILES ==================== #

# Prevents object files automatic deletion
.SECONDARY: $(TEST_OBJ)

# Target files
$(BIN_DIR)/%$(TARGET_EXT): $(BUILD_DIR)/$(TEST_DIR)/%.o $(OBJ_TARGET_EXCL)
	$(LD) $(LDFLAGS) $^ -o $@

# Object files (make-objs)
$(foreach bdir,$(TEST_BUILD_DIR),$(eval $(call make-target-objs,$(bdir))))

runtest: test
	@for x in bin/test_*; do ./$$x; done

memchecktest: test
	@for x in bin/test_*; do $(VALGRIND) $(VALGRIND_FLAGS) ./$$x; done

.PHONY: runtests memchecktest

# ==================== PROFILING ==================== #

gprof: default $(PROF_DIR)
	@make run
	$(GPROF) $(GPROF_FLAGS) $(BIN_DIR)/$(TARGET)$(TARGET_EXT) > $(PROF_DIR)/$(TARGET)_gprof.txt && $(EDITOR) $(PROF_DIR)/$(TARGET)_gprof.txt

gmon.out: default
	@make run

gcov: $(BUILD_DIR)/$(SRC_DIR)/$(TARGET).gcno
	$(GCOV) $(GCOV_FLAGS) $<
	@mv *.gcov $(PROF_DIR)
	@$(EDITOR) -o $(PROF_DIR)/*.gcov

lcov: $(BUILD_DIR)/$(SRC_DIR)/$(TARGET).gcno $(PROF_DIR)/html/index.html
	@xdg-open $(word 2,$^)

$(BUILD_DIR)/$(SRC_DIR)/$(TARGET).gcno: default
	@make run

$(PROF_DIR)/html/index.html: $(PROF_DIR)/cov.info
	@genhtml $< --output-directory=$(PROF_DIR)/html

$(PROF_DIR)/cov.info:
	$(LCOV) $(LCOV_FLAGS) --directory . --output-file=$@

.PHONY: gprof gcov lcov

# ==================== DIRECTORIES ==================== #

$(SRC_BUILD_DIR):
	@mkdir -p $@

$(TEST_BUILD_DIR):
	@mkdir -p $@

$(PROF_DIR):
	@mkdir -p $@

$(BIN_DIR):
	@mkdir -p $@

# ==================== CLEAN UP ==================== #

clean-prof:
	@rm -rf $(PROF_DIR)/*
	@rm -rf gmon.out

clean-out:
	@rm -rf $(OBJ) $(TEST_OBJ)
	@rm -rf $(BIN_DIR)/*

clean:
	make clean-prof
	make clean-out

cleanest:
	@rm -rf $(BUILD_DIR)
	@rm -rf $(BIN_DIR)
	@rm -rf $(PROF_DIR)
	@rm -rf gmon.out

.PHONY: clean-proj clean-out clean cleanest
