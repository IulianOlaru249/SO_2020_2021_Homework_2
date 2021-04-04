#ifndef _UTILS_H_
#define _UTILS_H_

#include "so_stdio.h"

#define DEFFILEMODE	0600
#define EINVAL		-22	/* Invalid argument */
#define EBADF		-9	/* Bad file number */

#define O_RDONLY	00000000
#define O_WRONLY	00000001
#define O_CREAT		00000100
#define O_TRUNC		00001000
#define O_APPEND	00002000
#define O_RDWR		00000002

#define _SRW		0x0010	/* open for reading & writing */
#define	_SRD		0x0004	/* OK to read */
#define _SWR		0x0008	/* OK to write */

int _sflags(const char *mode, int *optr);
SO_FILE *_sfp(void);
int _sgetc(SO_FILE *fp);

#endif
