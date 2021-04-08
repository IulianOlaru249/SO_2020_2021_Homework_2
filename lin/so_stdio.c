#include "utils.h"
#include <stdio.h>

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

	/* Set pid */
	fp->pid = -1;
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
	/* Set err code */
	fp->err = 0;
	/* Set empty checker */
	fp->_empty = 1;
	/* Set indexes for wread and write */
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
		return -EBADF;

	if (stream->_flags == 0)
		return SO_EOF;

	if (stream->_cookie == NULL)
		return SO_EOF;

	/* If there is data stored in the buffer */
	if (!stream->_empty)
		ret = so_fflush(stream);

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
	int ret = -1;

	ret = _sgetc(stream);

	if (ret == SO_EOF)
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

	/* Read nmemb elements */
	for (i = 0; i < nmemb; i++) {
		/* Read one element byte by byte */
		for (j = 0; j < size; j++) {
			/* Append elements to user buffer */
			byte = so_fgetc(stream);
			if (byte == -1 || ptr + offset == NULL || stream->_eof == SO_EOF)
				break;
			memccpy(ptr + offset, &byte, 1, sizeof(unsigned char));
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

	/* Set file cursor */
	stream->_file_cur = lseek(stream->_file, 0, SEEK_CUR) + (nmemb - stream->_length);

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
	int ret = 0;
	int res = 0;

	/* Handle the buffer */
	res = so_fflush(stream);
	if (res < 0)
		ret = -1;

	/* Move the file cursor */
	res = lseek(stream->_file, offset, whence);
	if (res < 0)
		ret = SO_EOF;

	/* Store the file cursor */
	stream->_file_cur = res;

	return ret;
}

long so_ftell(SO_FILE *stream)
{
	if (stream == NULL)
		return -1;

	return stream->_file_cur;
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

	ret = stream->err;

	return ret;
}

SO_FILE *so_popen(const char *command, const char *type)
{
	SO_FILE *fp = NULL;
	pid_t pid = -1;
	int pdes[2] = {0, 0};
	int ret = 0;
	int fd = 0;

	/* Check flags */
	if ((*type != 'r' && *type != 'w') || type[1] != '\0')
		return NULL;

	/* Open pipe */
	if (pipe(pdes) < 0)
		return NULL;

	/* Fork the process */
	pid = fork();
	switch (pid) {

	case -1:	/* Error */
		(void)close(pdes[0]);
		(void)close(pdes[1]);
		return NULL;
	case 0:		/* Child process */
		/* Redirect pipe */
		if (*type == 'r') {
			close(pdes[0]);
			if (pdes[1] != std_out) {
				ret = dup2(pdes[1], std_out);
				if (ret < 0)
					return NULL;
				close(pdes[1]);
			}
		} else {
			close(pdes[1]);
			if (pdes[0] != std_in) {
				ret = dup2(pdes[0], std_in);
				if (ret < 0)
					return NULL;
				close(pdes[0]);
			}
		}
		/* Execute the command */
		ret = execlp("/bin/sh", "sh", "-c", command, NULL);
		exit(ret);
	default:	/* Parent process */
		/* Redirect pipe */
		if (*type == 'r') {
			fd = pdes[0];
			close(pdes[1]);
		} else {
			fd = pdes[1];
			close(pdes[0]);
		}

		/* Open the file as normal */
		fp = _sfp();
		if (fp == NULL)
			return NULL;

		/* Set flags */
		fp->_flags = 1;
		/* Set fd */
		fp->_file = fd;
		/* Set pid */
		fp->pid = pid;
		/* Set eof checker */
		fp->_eof = 0;
		/* Set file pointer */
		fp->_cookie = fp;
		/* Set prev opperation */
		fp->_prev_op = NOOP;
		/* Set err code */
		fp->err = 0;
		/* Set empty checker */
		fp->_empty = 1;
		/* Set indexes for wread and write */
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
	return NULL;
}

int so_pclose(SO_FILE *stream)
{
	int ret = 0;
	int stat = 0;
	pid_t pid = -1;
	pid_t stream_pid = -1;

	if (stream == NULL)
		return -1;

	/* Store pid and close the stream */
	stream_pid = stream->pid;
	ret = so_fclose(stream);
	if (ret < 0)
		stat = 0;

	/* Wait for pid */
	pid = waitpid(stream_pid, &stat, 0);
	if (pid == -1)
		return SO_EOF;

	return stat;
}
