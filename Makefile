ifeq ($(shell uname -s), Darwin)
LDFLAGS=-L/usr/local/Cellar/glfw3/3.2.1 -framework OpenGL -lglfw3 -lGLEW -lm
else
LDFLAGS=-lX11 -lGL -lGLU -lglfw3 -lGLEW -lm
endif
CC = g++
CFLAGS=-g -Wall -std=c++11 -I/usr/local/Cellar/glfw3/3.2.1

TARGET = test
SRC = $(TARGET).cc

LIB = fin.o island.o read_ppm.o
all: test

test: $(SRC) $(LIB)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(TARGET) $(SRC) $(LIB)

fin.o: fin.cc fin.h
	$(CC) $(CFLAGS) -c fin.cc

island.o: island.cc island.h
	$(CC) $(CFLAGS) -c island.cc

read_ppm.o: read_ppm.c read_ppm.h
	$(CC) $(CFLAGS) -c read_ppm.c

clean:
	rm $(TARGET) $(LIB)
