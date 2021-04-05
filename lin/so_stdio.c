#include "utils.h"
#include <stdio.h>

/* ../../../SO_2020_2021_Homework_2/lin/ */

SO_FILE *so_fopen(const char *pathname, const char *mode)
{
	SO_FILE *fp = NULL;
	int f = 0;
	int oflags = 0, flags = 0;

	/* Get flags */
	flags = _sflags(mode, &oflags);
	if (flags == 0)
		return (NULL);

	/* Allocate memory */
	fp = _sfp();
	if (fp == NULL)
		return (NULL);

	/* Open stream */
	f = open(pathname, oflags, DEFFILEMODE);
	if (f < 0) {
		free(fp);
		return NULL;
	}

	/* Set file descriptor */
	fp->_file = f;
	/* Set flags */
	fp->_flags = flags;
	/* Set eof checker */
	fp->_eof = 0;
	/* Set file pointer */
	fp->_cookie = fp;
	/* Set prev opperation */
	fp->_prev_op = NOOP;
	/* Set empty checker */
	fp->_empty = 1;
	/* Set indexes for wread and write */
	fp->_br = fp->_buff;
	fp->_bw = fp->_buff;
	/* Set buffer */
	memset(fp->_buff, 0, FILE_BUFF_SIZE);

	return fp;
}

int so_fclose(SO_FILE *stream)
{
	int ret = 0;

	if (stream == NULL)
		return -EBADF;

	if (stream->_flags == 0)
		return SO_EOF;

	if (stream->_cookie == NULL)
		return SO_EOF;

	/* If there is data stored in the buffer */
	if (!stream->_empty)
		so_fflush(stream);

	if (close(stream->_file) < 0)
		ret = SO_EOF;

	free(stream);

	return ret;
}

int so_fileno(SO_FILE *stream)
{
	if (stream == NULL)
		return -EBADF;

	return stream->_file;
}

int so_fgetc(SO_FILE *stream)
{
	int ret = 0;

	ret = _sgetc(stream);

	return ret;
}

size_t so_fread(void *ptr, size_t size, size_t nmemb, SO_FILE *stream)
{
	int i = 0;
	int j = 0;
	unsigned char byte = 0;
	int offset = 0;
	int count = 0;

	/* Read nmemb elements */
	for (i = 0; i < nmemb; i++) {
		/* Read one element byte by byte */
		for (j = 0; j < size; j++) {
			/* Append elements to user buffer */
			byte = so_fgetc(stream);
			memccpy(ptr + offset, &byte, 1, sizeof(unsigned char));
			offset += sizeof(unsigned char);
		}
		count++;
	}
	
	return count;
}

int so_fputc(int c, SO_FILE *stream)
{
	int ret = 0;

	ret = _sputc(c, stream);

	return ret;
}

size_t so_fwrite(const void *ptr, size_t size, size_t nmemb, SO_FILE *stream)
{
	int i = 0;
	int j = 0;
	int byte = 0;
	int count = 0;
	unsigned char *aux = (unsigned char *)ptr;

	/* Read nmemb elements */
	for (i = 0; i < nmemb; i++) {
		/* Read one element byte by byte */
		for (j = 0; j < size; j++) {
			byte = *aux;
			so_fputc(byte, stream);
			aux++;
		}
		count++;
	}

	return count;
}

int so_fflush(SO_FILE *stream)
{
	/* As log as there is data in the buffer */
	if (stream->_prev_op == WRITE)
		xwrite(stream, stream->_bw - stream->_buff);
	
	/* Mark buffer as empty */
	stream->_empty = 1;
	/* Clear the memory from junk */
	memset(stream->_buff, 0, FILE_BUFF_SIZE);
	/* Reset read and write indexes */
	stream->_br = stream->_buff;
	stream->_bw = stream->_buff;

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
