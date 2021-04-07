CC = cl
PROJ_PATH = .
LIB_NAME = libso_stdio.dll
OBJECTS = so_stdio.obj
FLAGS = /nologo /W3 /EHsc

LIB = link
LIB_FLAGS = /dll /out:libso_stdio.dll /implib:libso_stdio.lib /nologo

build: $(LIB_NAME)

$(LIB_NAME): $(OBJECTS)
	cl /LD $**
	# $(LIB) $(LIB_FLAGS) $**

so_stdio.obj: $(PROJ_PATH)\so_stdio.c
	$(CC) $(FLAGS) /c $**

clean:
	del *.obj *.exe

