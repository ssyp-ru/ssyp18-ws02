#include <stdio.h>
#include <stdarg.h>
#define _POSIX_C_SOURCE 199309L
#define __USE_POSIX199309
#include <time.h>

FILE* lf = NULL;

void log_msg(const char* msg, ...) {
	lf = fopen("log.txt", "a");
	struct timespec start;
	clock_gettime(CLOCK_MONOTONIC, &start);
	fprintf(lf, "%ld.%ld / ", start.tv_sec, start.tv_nsec);
    va_list args;
	va_start(args, msg);
	vfprintf(lf, msg, args);
	va_end(args);
	fprintf(lf, "\n");
	fclose(lf);
	lf = NULL;
}
void log_err(const char* msg, ...) {
	lf = fopen("log.txt", "a");
	struct timespec start;
	clock_gettime(CLOCK_MONOTONIC, &start);
	fprintf(lf, "%ld.%ld / ", start.tv_sec, start.tv_nsec);
	fprintf(lf, "ERR: ");
    va_list args;
	va_start(args, msg);
	vfprintf(lf, msg, args);
	va_end(args);
	fprintf(lf, "\n");
	fclose(lf);
	lf = NULL;
}