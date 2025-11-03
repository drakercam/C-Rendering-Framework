CC = g++
CFLAGS = -Wall -Iinclude -std=c++17
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
