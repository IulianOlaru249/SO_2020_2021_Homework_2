Nume: Olaru Gabriel Iulian

Grupă: 334CC

# Assignment 2 SO STDIO

General Structure
-

***Code structure***
1. The goal of the project is to implement a version of the C standard input/output library.
2. The implementation is platform dependant. There is support for Windows (win32 SDK for windows 7) and Linux.

Structure
-

1. Code structure

├── LICENSE

Linux | Windows
------------ | -------------
 ├── lin                   |        └── win
 │   ├── GNUmakefile       |           ├── Makefile
 │   ├── so_stdio.c        |            ├── so_stdio.c
 │   ├── so_stdio.h        |            ├── so_stdio.h
 │   ├── utils.c           |            ├── utils.c
 │   └── utils.h           |            └── utils.h
 
 └── README.md            

** The Windows implementation does not support popen and pclose! **

***Flow***

The result of the project is a shared library which provides implementation for IO opperations.

All operations are buffered.

The library offers support for working with files:
  * Opening / Closing files.
  * Reading / Writing from files.
  * Adjusting the file cursor.

There is also support for creating processes:
  * A new process can be created to execute commands.
  * The process can, then, be closed safely.
  * The files created are available for read/ write opperations.

***Efficency***

All operations are buffered.
Read / Write is done bye by byte. If the buffer is empty, or there is not enough data,
then it is filled / emptied from / to a file.
This aproach limits the number of memory accesses, making the library fast.

***Corner cases***

The read/ write operations must be done in a loop since they can be interrupted anytime.
Doing this, we guarantee that we get the ammound of data that we asked for.

Implementation Details
-

The only diffrence between Linux and Windows is in the syscalls made.

Linux | Windows
------------ | -------------
open  | OpenFile
close | CloseHandle
write | WriteFile
read  | ReadFile
lseek | SetFilePointer


In memory, the file is modeled by a structure, described in so_stdio.h.

All the required features were Implemented.

Function | Location | Brief Description
------------ | ------------- | -------------
so_fopen | so_stdio.c | Gets the flags for opening a file, allocates memory for a file structure the opens the file
so_fclose | so_stdio.c | Flushes the buffer associated with the file, closes the opened stream and frees the memory of the file structure
so_fileno | so_stdio.c | Returns the file descriptor
so_fgetc | so_stdio.c | Gets a byte from the buffer associated with a file
so_fread | so_stdio.c | Gets n elements of specified size from a file (byte by byte -- is buffered)
so_putc | so_stdio.c | Puts a byte in the buffer associated with a file
so_fwrite | so_stdio.c | Puts n elements of specified size to a file (byte by byte -- is buffered)
so_fflush | so_stdio.c | Flushes the buffer associated to a file
so_fseek | so_stdio.c | Moves the cursor associated to a file
so_ftell | so_stdio.c | Returns the position of the file cursor
so_feof | so_stdio.c | Checks for eof
so_ferror | so_stdio.c | Checks for errors
so_popen | so_stdio.c | Openes a child process to execute command, redirecting the parent input/ output
so_pclose | so_stdio.c | Closes the file stream and waits for the child
------------ | ------------- | -------------
\_sflags | utils.c | Gets opening flags for a file
\_sfp | utils.c | Allocates memory for a file structure
xread | utils.c | Reads N bytes from a file
xwrite | utils.c | Writes N bytes to a file
\_sgetc | utiils.c | Gets a byte from the buffer
\_sputc | utils.c | Puts a byte in the buffer

Difficulties:

  * had to be really careful invalid reads / writes.

Fun Facts:
  * C89 still sucks.
  * spend half an our debugging because i compiled the library static instead of shared ಠ_ಠ .

How to run?
-

Three object files are linked:
  * hash_map.o -> the implementation of the hash map
  * utils.o -> contains implementation of all directive handles and other useful functions
  * main.o -> contains the main function, whitch controls the flow of the program

***To build***

The building process is not platform dependant, since each platform has its own Makefile

Platform | Makefile | Command
------------ | ------------- | -------------
Linux | GNUMakefile | make
Windows | Makefile | nmake

* Object following object files are created:
  * utils.o - has the implementation of *auxiliary functions* used by the library functions
  * so_stdio.o - has the implementation of the *library functions*
* The object files are linked in order to create the shared library *libso_stdio.so / .dll*
 
***To use***

The library libso_stdio.so / ( .dll + .lib ) must be linked to the target object.
The header so_unistd.h must be included.
  
Bibliography
-

* Useful Linux Links:
    * For the Makefiles: https://ocw.cs.pub.ro/courses/so/laboratoare/laborator-01
    * Learn from the pros: https://android.googlesource.com/platform/bionic/+/ics-mr0/libc/stdio
    * IO opperations under linux: https://ocw.cs.pub.ro/courses/so/laboratoare/laborator-02
    * File Data structure: https://sites.uclouvain.be/SystInfo/usr/include/asm-generic/fcntl.h.html
    * Processes under linux: https://ocw.cs.pub.ro/courses/so/laboratoare/laborator-03
    * Pipes: https://www.geeksforgeeks.org/pipe-system-call/
    * Linux errors: https://www-numi.fnal.gov/offline_software/srt_public_context/WebDocs/Errors/unix_system_errors.html

* Useful Windows Links:
    * Open files: https://docs.microsoft.com/en-us/windows/win32/fileio/opening-a-file-for-reading-or-writing
    * File management: https://www.installsetupconfig.com/win32programming/windowsfileapis4_24.html
    * File flags: https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-createfilea

Git
-

1. https://github.com/IulianOlaru249/SO_2020_2021_Homework_2/
