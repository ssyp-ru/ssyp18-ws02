#include "dumblogger.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void logs(char * message)
{
	static FILE * lf;

	if(!lf)
		lf = fopen("log.txt", "w");
	fputs(message, lf);
	fflush(lf);
}
/*
snprintf(message, "... %d %d", a,b);
log(message);
*/
