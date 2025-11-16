# Compilers
CC      = gcc
CXX     = g++

# Common flags
CFLAGS  = -g -O0 -Wall -Iinclude
CXXFLAGS= -g -O0 -Wall -Iinclude

# Libraries
CLIBS   = -lglfw -ldl -lGL -lm
CPPLIBS = -lglfw -ldl -lGL -lm -lassimp

# Source files
CSRC    = src/main.c src/glad.c
CPPSRC  = src/main.cpp src/glad.c

# Output files
COUT    = Framework_C
CPPOUT  = Framework_CPP

# Default target
all: $(COUT) $(CPPOUT)

# C build
$(COUT): $(CSRC)
	$(CC) $(CFLAGS) $(CSRC) -o $(COUT) $(CLIBS)

# C++ build
$(CPPOUT): $(CPPSRC)
	$(CXX) $(CXXFLAGS) $(CPPSRC) -o $(CPPOUT) $(CPPLIBS)

# Run targets
run_c:
	./$(COUT)

run_cpp:
	./$(CPPOUT)

# Clean
clean:
	rm -f $(COUT) $(CPPOUT)

# Convenience
go_c: $(COUT)
	./$(COUT)

go_cpp: $(CPPOUT)
	./$(CPPOUT)

reset: clean all
