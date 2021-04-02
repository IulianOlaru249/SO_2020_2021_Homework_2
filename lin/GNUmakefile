CC = gcc
CFLAGS = -Wall -g
PROJ_PATH =
EXEC = so-cpp
OBJECTS = main.o

LDFLAGS = 

build: $(EXEC)

$(EXEC): $(OBJECTS)
	$(CC) $^ $(LDFLAGS) -o $@

main.o: $(PROJ_PATH)/main.c
	$(CC) $(CFLAGS) $^ -c

clean:
	rm -f *.o so-cpp
