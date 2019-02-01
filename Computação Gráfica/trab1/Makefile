
# Author: Vinicius Ferraco Arruda
# Email:  viniciusferracoarruda@gmail.com
# Professor: Thiago Oliveira dos Santos
# Subject: Computer Graphics
# Assignment: Implementation of a circle with drag-and-drop and resize functions.

CC = gcc
#CFLAGS = -Wall -Wextra -Wconversion -pedantic
CFLAGS = -Wall -Wextra -g -ggdb
OBJ = src/color/color.o src/input/mouse/mouse.o src/shape/circle/circle.o src/shape/line/line.o src/window/window.o src/application/application.o src/xml_parser/xml_application_parser.o src/xml_parser/xml_parser.o src/main.o
LIBS = -lGL -lGLU -lglut -lm

all: trabalhocg

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS) 

trabalhocg: $(OBJ)
	$(CC) -o $@ $^ $(LIBS)
	$(MAKE) clean

clean:
	rm -f src/color/*.o
	rm -f src/input/mouse/*.o
	rm -f src/shape/circle/*.o
	rm -f src/shape/line/*.o
	rm -f src/window/*.o
	rm -f src/application/*.o
	rm -f src/xml_parser/*.o
	rm -f src/*.o

