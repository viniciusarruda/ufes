ABOUT :=                                                      \
	"                                                      \n \
	Author:     Vinicius Ferraco Arruda                    \n \
	Email:      viniciusferracoarruda@gmail.com            \n \
	Website:    viniciusarruda.github.io                   \n \
	Professor:  Thiago Oliveira dos Santos                 \n \
	Subject:    Computer Graphics                          \n \
	Assignment: 3D shooting game                           \n \
	                                                       \n \
	"
COMPILING := "Compiling "
DONE_COMPILING := "> Done !\n"
SEPARATOR := "-"
CLEANED := " All object files have been cleaned.\n\n"

SRC := $(shell find src -name '*.c')
OBJ := $(SRC:.c=.o)
CC := gcc
#CFLAGS = -Wall -Wextra -Wconversion -pedantic 
CFLAGS := -Wall -Wextra -g -ggdb 

LIBS := -lGL -lGLU -lglut -lm
EXEC := trabalhocg


all:
	@printf $(ABOUT)
	@printf $(COMPILING)
	@$(MAKE) -s $(EXEC)

debug: 
	@$(MAKE) -s all DEFINE=-DDEBUG_SENSOR

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS) $(DEFINE)
	@printf $(SEPARATOR)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(LIBS)
	@printf $(DONE_COMPILING)
	$(MAKE) clean
	@printf $(CLEANED)

clean: 
	rm -f $(OBJ)
