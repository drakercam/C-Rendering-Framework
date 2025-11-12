CC = g++
CFLAGS = -g -O0 -Wall -Iinclude
LIBS = -lglfw -ldl -lGL -lm -lassimp
SRC = src/main.cpp src/glad.c
OUT = C_Framework

all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(OUT) $(LIBS)

run:
	./$(OUT)

clean:
	rm -f $(OUT)

go:
	make && make run

reset:
	make clean && make && make run

memcheck:
	valgrind --leak-check=full \
         --show-leak-kinds=all \
         --suppressions=gl.supp \
         --suppressions=clean.supp \
         ./$(OUT)

