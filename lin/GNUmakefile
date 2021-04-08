CC = gcc
CFLAGS = -Wall -g  -fPIC -c
LDFLAGS = -lintro -L.

LIB_GEN = gcc
LIB_PARAM = -shared

PROJ_PATH = .
LIB_NAME = libso_stdio.so
OBJECTS = so_stdio.o utils.o

build: $(LIB_NAME)

$(LIB_NAME): $(OBJECTS)
	$(LIB_GEN) $(LIB_PARAM) $^ -o $@

so_stdio.o: $(PROJ_PATH)/so_stdio.c $(PROJ_PATH)/so_stdio.h
	$(CC) $(CFLAGS) $^

utils.o: $(PROJ_PATH)/utils.c $(PROJ_PATH)/utils.h
	$(CC) $(CFLAGS) $^

clean:
	rm -f *.o *.h.gch *.so
