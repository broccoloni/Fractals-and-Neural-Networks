CC = gcc
CFLAGS = -Wall

all: generatedata generatedata_no_cutoff

generatedata: generatedata.c Fractals.c vecio.c fracfuncs.c PNGio.c matvec_read.c
	        $(CC) $(CFLAGS) -o $@ $^ -lm -lpng -lpthread -ggdb

generatedata_no_cutoff: generatedata_no_cutoff.c Fractals.c vecio.c fracfuncs.c PNGio.c matvec_read.c
	        $(CC) $(CFLAGS) -o $@ $^ -lm -lpng -lpthread -ggdb
