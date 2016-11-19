ifeq ($(shell uname -s), Darwin)
LDFLAGS=-L/usr/local/Cellar/glfw3/3.2.1 -framework OpenGL -lglfw3 -lGLEW -lm
else
LDFLAGS=-lX11 -lGL -lGLU -lglfw3 -lGLEW -lm
endif
CC = g++
CFLAGS=-g -Wall -std=c++11 -I/usr/local/Cellar/glfw3/3.2.1

TARGET = simulate
SRC = $(TARGET).cc

LIB = initshader.o boid.o view.o goal.o read_ppm.o fin.o sun.o light.o ocean.o island.o
all: simulate

simulate: $(SRC) $(LIB)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(TARGET) $(SRC) $(LIB)

fin.o: fin.cc fin.h
	$(CC) $(CFLAGS) -c fin.cc

island.o: island.cc island.h
	$(CC) $(CFLAGS) -c island.cc

view.o: view.cc
	$(CC) $(CFLAGS) -c view.cc

goal.o: goal.cc
	$(CC) $(CFLAGS) -c goal.cc

sun.o: sun.cc
	$(CC) $(CFLAGS) -c sun.cc

ocean.o: ocean.cc
	$(CC) $(CFLAGS) -c ocean.cc

light.o: light.cc
	$(CC) $(CFLAGS) -c light.cc

initshader.o: initshader.c
	$(CC) $(CFLAGS) -c initshader.c

boid.o: boid.cc
	$(CC) $(CFLAGS) -c boid.cc

read_ppm.o: read_ppm.c
	$(CC) $(CFLAGS) -c read_ppm.c

clean:
	rm $(TARGET) $(LIB)
