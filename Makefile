
# Dep URLS

THREAD_POOL = https://github.com/progschj/ThreadPool.git

# GIT Commands

CLONE = git clone

# Directories

DEP = dep
BIN = bin
INSTALL_PATH = /usr/local

THREAD_POOL_DIR = ThreadPool

# Compiler Settings

CC = clang++
CXX_FLAGS = -std=c++17 -c
CXX_PROG_FLAGS = -std=c++17 
LINK_FLAGS = -I./$(DEP)/$(THREAD_POOL_DIR)

# SRC FILES 
SRC = src/du.cpp
SRC_PROG = src/mdu.cpp

# OBJ files

OBJECTS = du.o mdu.o

# NAMES

PROG_NAME = mdu

all: dep build

dep:
	mkdir -p $(DEP)
	if [ ! -d "$(DEP)/$(THREAD_POOL_DIR)" ]; then \
		$(CLONE) $(THREAD_POOL); \
		mv $(THREAD_POOL_DIR) $(DEP); \
	fi	

build:
	$(CC) $(CXX_FLAGS) $(LINK_FLAGS) $(SRC) 
	$(CC) $(CXX_FLAGS) $(LINK_FLAGS) $(SRC_PROG)
	$(CC) -o $(PROG_NAME) $(CXX_PROG_FLAGS) $(OBJECTS)
	mkdir -p $(BIN)
	mv $(PROG_NAME) $(BIN)


clean_all: clean_obj clean_dep
	rm -r $(BIN)

clean_obj:
	rm $(OBJECTS)

clean_dep:
	rm -rf $(DEP)
install:
	cp $(BIN)/$(PROG_NAME) $(INSTALL_PATH)
