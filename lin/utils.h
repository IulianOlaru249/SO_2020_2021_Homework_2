#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#include "so_stdio.h"

#define DEFFILEMODE	0600
#define EINVAL		22	/* Invalid argument */
#define EBADF		9	/* Bad file number */

#define _SRW		0x0010	/* open for reading & writing */
#define	_SRD		0x0004	/* OK to read */
#define _SWR		0x0008	/* OK to write */

int _sflags(const char *mode, int *optr);
SO_FILE *_sfp(void);
int _sgetc(SO_FILE *fp);
int _sputc(int c, SO_FILE *fp);
int xread(SO_FILE *fp, int count);
int xwrite(SO_FILE *fp, size_t count);

#endif
