CC = g++
CFLAGS = -Wall
PROG = arachnophobia

SRCS = arachnophobia.cpp
LIBS = -lglut -lGL -lGLU `sdl-config --cflags --libs` -lSDL_mixer

all: $(PROG)

$(PROG):	$(SRCS)
	$(CC) $(CFLAGS) -o $(PROG) $(SRCS) $(LIBS)

clean:
	rm -f $(PROG)
