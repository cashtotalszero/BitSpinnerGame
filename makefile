CFLAGS = `sdl2-config --cflags` -O3 -Wall -pedantic -std=c99 -lm
INCS = bitspinner.h
TARGET = bitspinner
SOURCES =  bitspinnerSDL_MAIN.c bitspinnerSDL_EVENTS.c bitspinnerSDL_BLITS.c bitspinnerGI.c bitspinnerMODES.c $(TARGET).c
LIBS =  `sdl2-config --libs` 
CC = gcc


all: $(TARGET)

$(TARGET): $(SOURCES) $(INCS)
	$(CC) $(SOURCES) -o $(TARGET) $(CFLAGS) $(LIBS)

clean:
	rm -f $(TARGET)

run: all
	$(TARGET)
