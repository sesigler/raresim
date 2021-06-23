BIN=../bin
OBJ=../obj
CFLAGS=-g
#CFLAGS=-O2
CFLAGS+=-D_FILE_OFFSET_BITS=64 -Werror -Wuninitialized
CC=gcc

%.o: %.c 
	$(CC) -c -o $@ $< $(CFLAGS)

read: read.o lists.o
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o read
