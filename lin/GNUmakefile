CC = gcc
CFLAGS = -Wall -g -c
LDFLAGS = -lintro -L.

LIB_GEN=ar
LIB_PARAM = rc

PROJ_PATH = .
LIB_NAME = libso_stdio.so
OBJECTS = so_stdio.o

build: $(LIB_NAME)

$(LIB_NAME): $(OBJECTS)
	$(LIB_GEN) $(LIB_PARAM) $@ $^

so_stdio.o: $(PROJ_PATH)/so_stdio.c $(PROJ_PATH)/so_stdio.h
	$(CC) $(CFLAGS) $^ $(LDFLAGS)

clean:
	rm -f *.o *.h.gch *.so
