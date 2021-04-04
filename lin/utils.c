#include "utils.h"
#include <unistd.h>
#include "stdio.h"

int _sflags(const char *mode, int *optr)
{
	int ret = 0;
	int m = 0;
	int o = 0;
	char c = *mode++;

	switch (c) {

	case 'r':	/* Open for reading */
		ret = _SRD;
		m = O_RDONLY;
		o = 0;
		break;
	case 'w':	/* Open for writing */
		ret = _SWR;
		m = O_WRONLY;
		o = O_CREAT | O_TRUNC;
		break;
	case 'a':	/* Open for appending */
		ret = _SWR;
		m = O_WRONLY;
		o = O_CREAT | O_APPEND;
		break;
	default:	/* Illegal */
		return 0;
	}

	if (*mode == '+' || (*mode == 'b' && mode[1] == '+')) {
		ret = _SRW;
		m = O_RDWR;
	}

	*optr = m | o;
	return ret;
}

SO_FILE *_sfp(void)
{
	SO_FILE *fp = NULL;

	fp = (SO_FILE *)malloc(sizeof(struct _so_file));
	if (fp == NULL)
		return NULL;

	return fp;
}

int _sgetc(SO_FILE *fp)
{
	int ret = 0;
	int bytes_read = -1;

        /* If the buffer is empty or there is not enough data, refresh */
        if (fp->_empty || fp->_br - fp->_buff > FILE_BUFF_SIZE - 1) {
				printf("!!!!!\n");
                /* Reaattempt operation as long sa it returns 0 */
                do {
	                bytes_read = read(fp->_file, fp->_buff, FILE_BUFF_SIZE);
                } while (bytes_read == 0);
                /* Mark that the buffer is not empty anymore */
                fp->_empty = 0;
                /* Reset the position in the buffer */
                fp->_br = fp->_buff;
        }

        /* Get the value from the buffer and advance the position */
        ret = *fp->_br;
        fp->_br++;

        return ret;
}