CC = gcc
CPP = g++
LD = gcc
RM = /bin/rm -f
CFLAGS = -Wall -O2
LDFLAGS = -I/usr/X11R6/include -L/usr/X11R6/lib -lGL -lGLU -lglut -lm -lstdc++

SRC = Main.cc Camera.cc Vector.cc
OBJ = $(SRC:.cc=.o)

PROG = main

all: $(PROG)

$(PROG): $(OBJ)
	$(LD) $(LDFLAGS) $(OBJ) -o $(PROG)

$(OBJ): $(SRC)
	$(CPP) $(CFLAGS) -c $(SRC)

clean:
	$(RM) $(OBJ) $(PROG)
