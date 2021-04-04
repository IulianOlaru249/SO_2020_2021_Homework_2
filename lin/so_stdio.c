#include "so_stdio.h"
#include "utils.h"
#include <fcntl.h>
#include <unistd.h>
#include "stdlib.h"
#include <string.h>
#include "stdio.h"

/* ../../../SO_2020_2021_Homework_2/lin/ */

SO_FILE *so_fopen(const char *pathname, const char *mode)
{
	SO_FILE *fp = NULL;
	int f = 0;
	int oflags = 0, flags = 0;

	flags = _sflags(mode, &oflags);
	if (flags == 0)
		return (NULL);

	fp = _sfp();
	if (fp == NULL)
		return (NULL);

	f = open(pathname, oflags, DEFFILEMODE);
	if (f < 0) {
		free(fp);
		return NULL;
	}

	fp->_file = f;
	fp->_flags = flags;
	fp->_cookie = fp;
	fp->_empty = 1;
	fp->_br = fp->_buff;
	memset(fp->_buff, 0, FILE_BUFF_SIZE);

	return fp;
}

int so_fclose(SO_FILE *stream)
{
	int ret = 0;

	if (stream == NULL)
		return EBADF;

	if (stream->_flags == 0)
		return SO_EOF;

	if (stream->_cookie == NULL)
		return SO_EOF;

	if (close(stream->_file) < 0)
		ret = SO_EOF;

	free(stream);

	return ret;
}

int so_fileno(SO_FILE *stream)
{
	if (stream == NULL)
		return EBADF;

	return stream->_file;
}

int so_fgetc(SO_FILE *stream)
{
	int ret = 0;

	ret = _sgetc(stream);

	return ret;
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

size_t so_fread(void *ptr, size_t size, size_t nmemb, SO_FILE *stream)
{
	return 0;
}

size_t so_fwrite(const void *ptr, size_t size, size_t nmemb, SO_FILE *stream)
{
	return 0;
}

int so_fputc(int c, SO_FILE *stream)
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
