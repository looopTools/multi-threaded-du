
# Dep URLS

THREAD_POOL = https://github.com/progschj/ThreadPool.git

# GIT Commands

CLONE = git clone

# Directories

DEP = dep

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

dep:
	mkdir -p $(DEP)
	cd $(DEP) 
	$(CLONE) $(THREAD_POOL)
	mv $(THREAD_POOL_DIR) $(DEP)

build:
	$(CC) $(CXX_FLAGS) $(LINK_FLAGS) $(SRC) 
	$(CC) $(CXX_FLAGS) $(LINK_FLAGS) $(SRC_PROG)
	$(CC) -o $(PROG_NAME) $(CXX_PROG_FLAGS) $(OBJECTS) 

