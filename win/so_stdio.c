#include "so_stdio.h"

SO_FILE *so_fopen(const char *pathname, const char *mode)
{
	// CREATE_ALWAYS

	SO_FILE *fp = NULL;
	HANDLE fHandle;
	long oflags1 = 0, oflags2 = 0, flags = 0;
	char c = *mode++;
	char c2 = *mode;

	/* Allocate memory */
	fp = _sfp();
	if (fp == NULL)
		return NULL;

	/* Open Stream */
	switch (c) {

	case 'r':	/* Open for reading */

		if (c2 == '+')
			/* Open stream */
			fHandle = CreateFile(
				pathname,
				GENERIC_ALL,		/* access mode */
				0, 			/* sharing option */
				NULL,			/* security attributes */
				OPEN_EXISTING,		/* open only if it exists */
				0,
				NULL
			);
		else
			/* Open stream */
			fHandle = CreateFile(
				pathname,
				GENERIC_READ,		/* access mode */
				0, 	/* sharing option */
				NULL,			/* security attributes */
				OPEN_EXISTING,		/* open only if it exists */
				0,
				NULL
			);

		break;
	case 'w':	/* Open for writing */

		if (c2 == '+')
			/* Open stream */
			fHandle = CreateFile(
				pathname,
				GENERIC_ALL,		/* access mode */
				0, 			/* sharing option */
				NULL,			/* security attributes */
				OPEN_ALWAYS,		/* open only if it exists */
				0,
				NULL
			);
		else
			/* Open stream */
			fHandle = CreateFile(
				pathname,
				GENERIC_WRITE,		/* access mode */
				0, 			/* sharing option */
				NULL,			/* security attributes */
				CREATE_ALWAYS,
				FILE_ATTRIBUTE_NORMAL,	/* file attributes */
				NULL
			);

		break;
	case 'a':	/* Open for appending */

		if (c2 == '+')
			/* Open stream */
			fHandle = CreateFile(
				pathname,
				GENERIC_READ | FILE_APPEND_DATA,
				FILE_SHARE_READ,
				NULL,			/* security attributes */
				OPEN_ALWAYS,
				FILE_ATTRIBUTE_NORMAL,	/* file attributes */
				NULL
			);
		else
			/* Open stream */
			fHandle = CreateFile(
				pathname,
				GENERIC_ALL,		/* access mode */
				0, 			/* sharing option */
				NULL,			/* security attributes */
				OPEN_ALWAYS,		/* open only if it exists */
				0,
				NULL
			);

		break;
	default:	/* Illegal */
		free(fp);
		return NULL;
	}


	if (fHandle == INVALID_HANDLE_VALUE) {
		free(fp);
		return NULL;
	}

	/* Set pid */
	fp->pid = -1;
	/* Set eof */
	fp->_eof = 0;
	/* Set file descriptor */
	fp->_file = fHandle;
	/* Set flags */
	fp->_flags = 1;
	/* Set file pointer */
	fp->_cookie = fp;
	/* Set prev op */
	fp->_prev_op = NOOP;
	/* Set err code */
	fp->_err = 0;
	/* Set empty checker */
	fp->_empty = 1;
	/* Set indexes for read/write */
	fp->_br = fp->_buff;
	fp->_bw = fp->_buff;
	/* Set buffer */
	memset(fp->_buff, 0, FILE_BUFF_SIZE);
	/* Set file cursor */
	fp->_file_cur = 0;
	/* Set length */
	fp->_length = 0;

	return fp;
}

int so_fclose(SO_FILE *stream)
{
	int ret = 0;

	if (stream == NULL)
		return -1;

	if (stream->_flags == 0)
		return -1;

	if (stream->_cookie == NULL)
		return -1;

	if (!stream->_empty)
		ret = so_fflush(stream);

	if (CloseHandle(stream->_file) == 0)
		return -1;

	free(stream);

    	return ret;
}

HANDLE so_fileno(SO_FILE *stream)
{
	if (stream == NULL)
		return -1;

	return stream->_file;
}

int so_fgetc(SO_FILE *stream)
{
	int ret = -1;

	ret = _sgetc(stream);

	if(ret == -1)
		stream->_eof = SO_EOF;

	return ret;
}

size_t so_fread(void *ptr, size_t size, size_t nmemb, SO_FILE *stream)
{
	int i = 0;
	int j = 0;
	unsigned char byte = 0;
	int offset = 0;
	int count = 0;
	unsigned char *aux = (unsigned char *)ptr;

	/* Read nmemb elements */
	for (i = 0; i < nmemb; i++) {
		/* Read one element byte by byte */
		for (j = 0; j < size; j++) {
			/* Append elements to user buffer */
			byte = so_fgetc(stream);
			if (byte == -1 || aux + offset == NULL || stream->_eof == SO_EOF)
				break;
			memccpy(aux + offset, &byte, 1, sizeof(unsigned char));
			offset += sizeof(unsigned char);
		}

		if (so_feof(stream)) {
			stream->_file_cur = count;
			return count;
		}

		count++;
	}

	/* Set file cursor */
	stream->_file_cur = count;
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
	int ret = 0;
	unsigned char *aux = (unsigned char *)ptr;

	if (stream->_eof == SO_EOF)
		return 0;

	/* Read nmemb elements */
	for (i = 0; i < nmemb; i++) {
		/* Read one element byte by byte */
		for (j = 0; j < size; j++) {
			byte = *aux;
			ret = so_fputc(byte, stream);
			if (ret == -1)
				break;

			if (so_ferror(stream))
				return SO_EOF;
			aux++;
		}
		count++;
	}

	return count;
}

int so_fflush(SO_FILE *stream)
{
	int ret = 0;
	int bytes_written = 0;
	/* If the laso opration before flush was write */
	if (stream->_prev_op == WRITE) {
		/* Write all contents */
		bytes_written = xwrite(stream, stream->_bw - stream->_buff);
		// printf("%d\n", bytes_written);
		if (bytes_written != stream->_bw - stream->_buff)
			ret = -1;
	}

	/* Mark buffer as empty */
	stream->_eof = 0;
	stream->_empty = 1;
	stream->_length = 0;
	/* Clear the memory from junk */
	memset(stream->_buff, 0, FILE_BUFF_SIZE);
	/* Reset read and write indexes */
	stream->_br = stream->_buff;
	stream->_bw = stream->_buff;

	return ret;
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
	if (stream == NULL)
		return -2;

	return stream->_eof;
}

int so_ferror(SO_FILE *stream)
{
	int ret = 0;

	if (stream == NULL)
		return -1;

	ret = stream->_err;

	return ret;
}

SO_FILE *so_popen(const char *command, const char *type)
{
    return NULL;
}

int so_pclose(SO_FILE *stream)
{
    return 0;
}