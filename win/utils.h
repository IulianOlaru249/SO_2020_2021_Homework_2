#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#include "so_stdio.h"
#include <stdio.h>

#define _SRW    0x0010    /* Open for writing+reading */
#define _SRD    0x0004    /* Open for reading */
#define _SWR    0x0008    /* Open for writing */

#define std_in  0
#define std_out 1

SO_FILE *_sfp(void);
int xread(SO_FILE *fp, int count);
int _sgetc(SO_FILE *fp);
int xwrite(SO_FILE *fp, size_t count);
int _sputc(int c, SO_FILE *fp);


#endif
