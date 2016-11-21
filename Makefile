ifeq ($(shell uname -s), Darwin)
LDFLAGS=-L/opt/ports/lib -framework OpenGL -lglfw -lGLEW -lm
else
LDFLAGS=-lX11 -lGL -lGLU -lglfw -lGLEW -lm
endif
CC = g++
CFLAGS=-g -Wall -std=c++11 -I/opt/ports/include

TARGET = simulate
SRC = $(TARGET).cc

LIB = initshader.o boid.o view.o goal.o read_ppm.o fin.o sun.o light.o ocean.o island.o octopus.o anti_collision.o
all: simulate

simulate: $(SRC) $(LIB)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(TARGET) $(SRC) $(LIB)

fin.o: fin.cc fin.h
	$(CC) $(CFLAGS) -c fin.cc

island.o: island.cc island.h
	$(CC) $(CFLAGS) -c island.cc

view.o: view.cc view.h
	$(CC) $(CFLAGS) -c view.cc

goal.o: goal.cc goal.h
	$(CC) $(CFLAGS) -c goal.cc

sun.o: sun.cc
	$(CC) $(CFLAGS) -c sun.cc

octopus.o: octopus.cc
	$(CC) $(CFLAGS) -c octopus.cc

ocean.o: ocean.cc
	$(CC) $(CFLAGS) -c ocean.cc

light.o: light.cc light.h
	$(CC) $(CFLAGS) -c light.cc

initshader.o: initshader.c
	$(CC) $(CFLAGS) -c initshader.c

boid.o: boid.cc boid.h
	$(CC) $(CFLAGS) -c boid.cc

read_ppm.o: read_ppm.c read_ppm.h
	$(CC) $(CFLAGS) -c read_ppm.c

anti_collision.o: anti_collision.cc anti_collision.h
	$(CC) $(CFLAGS) -c anti_collision.cc

clean:
	rm $(TARGET) $(LIB)
