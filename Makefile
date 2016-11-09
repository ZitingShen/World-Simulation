ifeq ($(shell uname -s), Darwin)
LDFLAGS=-L/opt/ports/lib -framework OpenGL -lglfw -lGLEW -lm
else
LDFLAGS=-lX11 -lGL -lGLU -lglfw -lGLEW -lm
endif
CC = g++
CFLAGS=-g -Wall -std=c++11 -I/opt/ports/include

TARGET = simulate
SRC = $(TARGET).cc

LIB = initshader.o
all: simulate

mesh: $(SRC) $(LIB)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(TARGET) $(SRC) $(LIB)

initshader.o: initshader.c
	$(CC) $(CFLAGS) -c initshader.c

clean:
	rm $(TARGET) $(LIB)
