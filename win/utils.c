#include "utils.h"

SO_FILE *_sfp(void)
{
	SO_FILE *fp = NULL;

	fp = (SO_FILE *)malloc(sizeof(struct _so_file));
	if (fp == NULL)
		return NULL;

	return fp;
}

int xread(SO_FILE *fp, int count)
{
	int ret = 0;
	int bytes_read = 0;
	int bytes_read_now = 0;

	while (bytes_read < count) {
		ret = ReadFile(fp->_file,
			fp->_buff + bytes_read,
			count - bytes_read_now,
			&bytes_read_now,
			NULL);

		/* EOF detected */
		if (bytes_read_now == 0) {
			fp->_eof = SO_EOF;
			break;
		}

		/* IO error */
		if (bytes_read_now < 0 || ret == 0)
			break;

		bytes_read += bytes_read_now;

		/* Interrupted */
		if (bytes_read_now < count)
			break;
	}

	return bytes_read;
}

int _sgetc(SO_FILE *fp)
{
	int ret = 0;

	/* If prev operation was write, flush the buffer */
	if (fp->_prev_op == WRITE)
		so_fflush(fp);

	/* If buffer is empty, or there is not enough data, refill */
	if (fp->_empty || fp->_br - fp->_buff > fp->_length - 1) {
		/* Clear junk */
		memset(fp->_buff, 0 ,FILE_BUFF_SIZE);
		/* Refill */
		fp->_length = xread(fp, FILE_BUFF_SIZE);
		if (fp->_length == 0)
			return -1;
		/* Mark the buffer is not empty anymore */
		fp->_empty = 0;
		/* Reset the reading cursor */
		fp->_br = fp->_buff;
	}

	/* Mark prev operation as read */
	fp->_prev_op = READ;
	/* Get the value from the buffer */
	ret = (unsigned char)*fp->_br;
	/* Advance reading cursor */
	fp->_br++;

	return ret;
}

int xwrite(SO_FILE *fp, size_t count)
{
	int ret = 0;
	int bytes_written = 0;
	int bytes_written_now = 0;

	while (bytes_written < count) {
		ret = WriteFile(fp->_file,
			fp->_buff + bytes_written,
			count - bytes_written_now,
			&bytes_written_now,
			NULL);

		if (bytes_written_now <= 0 || ret == 0) {
			fp->_err = SO_EOF;
			break;
		}

		bytes_written += bytes_written_now;
	}

	return bytes_written;
}

int _sputc(int c, SO_FILE *fp)
{
	int ret = c;

	/* If the previous operation was a read, flush the buffer */
	if (fp->_prev_op == READ)
		so_fflush(fp);


	/* When buffer gets full */
	if (fp->_bw - fp->_buff == FILE_BUFF_SIZE) {
		/* Write the buffer */
		fp->_length = xwrite(fp, FILE_BUFF_SIZE);
		/* Clear junk */
		memset(fp->_buff, 0, FILE_BUFF_SIZE);
		/* Reset the writing cursor to the buffer start */
		fp->_bw = fp->_buff;
		/* Mark that the buffer is empty again */
		fp->_empty = 1;
	}

	/* Mark that the buffer is not empty anymore */
	fp->_empty = 0;
	/* Mark the previous opperation as a write */
	fp->_prev_op = WRITE;
	/* Add the caracter to the buffer */
	memcpy(fp->_bw, &c, sizeof(unsigned char));
	/* Advance the writing cursor */
	fp->_bw++;

	return ret;
}