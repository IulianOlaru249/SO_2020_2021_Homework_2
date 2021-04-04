CC = gcc
CFLAGS = -Wall -g -c
LDFLAGS = -lintro -L.

LIB_GEN=ar
LIB_PARAM = rc

PROJ_PATH = .
LIB_NAME = libso_stdio.so
OBJECTS = so_stdio.o utils.o

build: $(LIB_NAME)

$(LIB_NAME): $(OBJECTS)
	$(LIB_GEN) $(LIB_PARAM) $@ $^

so_stdio.o: $(PROJ_PATH)/so_stdio.c $(PROJ_PATH)/so_stdio.h
	$(CC) $(CFLAGS) $^ $(LDFLAGS)

utils.o: $(PROJ_PATH)/utils.c $(PROJ_PATH)/utils.h
	$(CC) $(CFLAGS) $^ $(LDFLAGS)

clean:
	rm -f *.o *.h.gch *.so

test: main.o
	gcc -L. -o main main.o -lso_stdio

main.o:
	gcc -Wall -g -O0 -c -o main.o main.c