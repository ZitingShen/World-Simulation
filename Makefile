ifeq ($(shell uname -s), Darwin)
LDFLAGS=-L/opt/ports/lib -framework OpenGL -lglfw -lGLEW -lm
else
LDFLAGS=-lX11 -lGL -lGLU -lglfw -lGLEW -lm
endif
CC = g++
CFLAGS=-g -Wall -std=c++11 -I/opt/ports/include

TARGET = simulate
SRC = $(TARGET).cc

LIB = anti_collision.o boid.o fin.o goal.o initshader.o island.o light.o ocean.o  octopus.o read_ppm.o sun.o tree.o view.o 
all: simulate

simulate: $(SRC) $(LIB)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(TARGET) $(SRC) $(LIB)

anti_collision.o: anti_collision.cc anti_collision.h
	$(CC) $(CFLAGS) -c anti_collision.cc

boid.o: boid.cc boid.h
	$(CC) $(CFLAGS) -c boid.cc

fin.o: fin.cc fin.h
	$(CC) $(CFLAGS) -c fin.cc

goal.o: goal.cc goal.h
	$(CC) $(CFLAGS) -c goal.cc

initshader.o: initshader.c
	$(CC) $(CFLAGS) -c initshader.c

island.o: island.cc island.h
	$(CC) $(CFLAGS) -c island.cc

light.o: light.cc light.h
	$(CC) $(CFLAGS) -c light.cc

ocean.o: ocean.cc
	$(CC) $(CFLAGS) -c ocean.cc

octopus.o: octopus.cc
	$(CC) $(CFLAGS) -c octopus.cc

read_ppm.o: read_ppm.c read_ppm.h
	$(CC) $(CFLAGS) -c read_ppm.c

sun.o: sun.cc
	$(CC) $(CFLAGS) -c sun.cc

tree.o: tree.cc
	$(CC) $(CFLAGS) -c tree.cc

view.o: view.cc view.h
	$(CC) $(CFLAGS) -c view.cc

clean:
	rm $(TARGET) $(LIB)
