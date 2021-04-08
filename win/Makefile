# CC = cl
# PROJ_PATH = .
# LIB_NAME = so_stdio.dll
# OBJECTS = so_stdio.obj
# FLAGS = /nologo /W3 /c

# LIB = link
# LIB_FLAGS = /nologo /dll /out:so_stdio.dll /implib:so_stdio.lib

# build: so_stdio.dll

# so_stdio.dll: so_stdio.obj
# 	link /nologo /dll /out:so_stdio.dll /implib:so_stdio.lib so_stdio.obj

# so_stdio.obj: so_stdio.c
# 	cl /nologo /W3 /c /DDLL_EXPORTS so_stdio.c

build:
	cl /nologo /W3 /c /MD /DDLL_EXPORT utils.c
	cl /nologo /W3 /c /MD /DDLL_EXPORT so_stdio.c
	link /nologo /dll /out:so_stdio.dll /implib:so_stdio.lib so_stdio.obj utils.obj

clean:
	del *.obj *.exe so_stdio.dll so_stdio.lib so_stdio.exp
