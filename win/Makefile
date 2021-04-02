CC = cl
PROJ_PATH =
EXEC =
OBJECTS = main.obj
FLAGS = /MD

build: $(EXEC)

$(EXEC): $(OBJECTS)
	$(CC) $(FLAGS) /Fe$@ $**

main.obj: $(PROJ_PATH)\main.c
	$(CC) $(FLAGS) /c $**

clean:
	del *.obj *.exe

