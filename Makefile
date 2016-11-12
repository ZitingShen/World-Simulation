ifeq ($(shell uname -s), Darwin)
LDFLAGS=-L/opt/ports/lib -framework OpenGL -lglfw -lGLEW -lm
else
LDFLAGS=-lX11 -lGL -lGLU -lglfw -lGLEW -lm
endif
CC = g++
CFLAGS=-g -Wall -std=c++11 -I/opt/ports/include

TARGET = simulate
SRC = $(TARGET).cc

LIB = initshader.o boid.o view.o goal.o read_ppm.o fin.o
all: simulate

simulate: $(SRC) $(LIB)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(TARGET) $(SRC) $(LIB)

fin.o: fin.cc
	$(CC) $(CFLAGS) -c fin.cc

boid.o: boid.cc
	$(CC) $(CFLAGS) -c boid.cc

view.o: view.cc
	$(CC) $(CFLAGS) -c view.cc

goal.o: goal.cc
	$(CC) $(CFLAGS) -c goal.cc

initshader.o: initshader.c
	$(CC) $(CFLAGS) -c initshader.c

read_ppm.o: read_ppm.c
	$(CC) $(CFLAGS) -c read_ppm.c

clean:
	rm $(TARGET) $(LIB)
