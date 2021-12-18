#ifndef	__PRINT_H
#define __PRINT_H

#include "canif.h"


/********************************************************************/
#if (DEBUG_PRINT_ == 1)

#define   DEBUG_ID   0x777

void out_char(char *ch, int length);

int
char_present(void);

int		
printf(const char *, ... );

int
sprintf(char *, const char *, ... );


#else
#define printf(str, ...)
#endif
/********************************************************************/







#endif
