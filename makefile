CC = gcc
CFLAGS = -Wall

all: generatedata

generatedata: generatedata.c Fractals.c vecio.c fracfuncs.c PNGio.c matvec_read.c
	        $(CC) $(CFLAGS) -o $@ $^ -lm -lpng -lpthread -ggdb

