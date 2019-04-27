// Author: Denis Roman
// Date: April 27, 2019
// Made for SOU Hackathon 2019

#ifndef STDIO_H
#define STDIO_H
#include <stdio.h>
#endif

#ifndef STDLIB_H
#define STDLIB_H
#include <stdlib.h>
#endif

#define FILE_BUFFER_SIZE 65536*16 // 1 mb
#define ERROR_FILE_OPEN -2
#define ERROR_FILE_READ -3
#define ERROR_FILE_WRITE -4
#define ERROR_MALLOC -5
#define ERROR_REALLOC -6

typedef struct {
	long length;
	unsigned char *data;
} FileData;

int ReadFileData(FileData *f, const char *filename, const char *mode);
int WriteFileData(FileData *f, const char *filename, const char *mode);
const char *TranslateError(int error);
