#include "so_stdio.h"

SO_FILE *so_fopen(const char *pathname, const char *mode)
{
    return NULL;
}

int so_fclose(SO_FILE *stream)
{
    return 0;
}

HANDLE so_fileno(SO_FILE *stream)
{
    return NULL;   
}

size_t so_fread(void *ptr, size_t size, size_t nmemb, SO_FILE *stream)
{
    return 0;
}

int so_fgetc(SO_FILE *stream)
{
    return 0;
}

size_t so_fwrite(const void *ptr, size_t size, size_t nmemb, SO_FILE *stream)
{
    return 0;
}

int so_fflush(SO_FILE *stream)
{
    return 0;
}

int so_fseek(SO_FILE *stream, long offset, int whence)
{
    return 0;
}

long so_ftell(SO_FILE *stream)
{
    return 0;
}

int so_feof(SO_FILE *stream)
{
    return 0;
}

int so_ferror(SO_FILE *stream)
{
    return 0;
}

SO_FILE *so_popen(const char *command, const char *type)
{
    return NULL;
}

int so_pclose(SO_FILE *stream)
{
    return 0;
}