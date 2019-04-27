// Author: Denis Roman
// Date: April 27, 2019
// Made for SOU Hackathon 2019

#include "fileutil.h"

int ReadFileData(FileData *f, const char *filename, const char *mode) {
	
	FILE *inputfile = fopen(filename, mode);
	if (!inputfile) return ERROR_FILE_OPEN;
	
	// hack to detect seek_end support
	// shouldn't use with nonbinary files
	if (mode[1] == 'b') {
		// file is binary
		if (!fseek(inputfile, 0, SEEK_END)) {
			// fseek succeeded?
			long filesize = ftell(inputfile);
			rewind(inputfile);
			if (filesize > 0) {
				unsigned char *data = (unsigned char *) malloc(sizeof(unsigned char)*filesize);
				if (!data) {
					fclose(inputfile);
					return ERROR_MALLOC;
				}
				
				if (fread(data, sizeof(unsigned char), filesize, inputfile)*sizeof(unsigned char) == filesize) {
					fclose(inputfile);
					f->length = filesize;
					f->data = data;
					
					return 0;
				} else {
					free(data);
				}
			}
		} else {
			rewind(inputfile);
		}
	}
	
	unsigned char *data = (unsigned char *) malloc(sizeof(unsigned char) * FILE_BUFFER_SIZE);
	if (!data) {
		fclose(inputfile);
		return ERROR_MALLOC;
	}
	long totalsize = FILE_BUFFER_SIZE;
	
	
	// start reading file
	long numread = fread(data, sizeof(unsigned char), FILE_BUFFER_SIZE, inputfile);
	if (ferror(inputfile)) {
		free(data);
		fclose(inputfile);
		return ERROR_FILE_READ;
	}
	if (numread == 0) {
		free(data);
		fclose(inputfile);
		f->length = 0;
		f->data = NULL;
		return 0;
	}
	if (feof(inputfile)) {
		unsigned char *temp = realloc(data, sizeof(unsigned char) * numread);
		if (!temp) {
			free(data);
			fclose(inputfile);
			return ERROR_REALLOC;
		}
		data = temp;
		fclose(inputfile);
		f->length = numread;
		f->data = data;
		return 0;
	}
	
	while (1) {
		// extend buffer
		unsigned char *temp = realloc(data, sizeof(unsigned char) * (totalsize + FILE_BUFFER_SIZE));
		if (!temp) {
			free(data);
			fclose(inputfile);
			return ERROR_REALLOC;
		}
		data = temp;
		
		// read more data
		numread = fread(data+totalsize, sizeof(unsigned char), FILE_BUFFER_SIZE, inputfile);
		if (ferror(inputfile)) {
			free(data);
			fclose(inputfile);
			return ERROR_FILE_READ;
		}
		if (feof(inputfile)) {
			// realloc buffer
			unsigned char *temp = realloc(data, sizeof(unsigned char) * (totalsize + numread));
			if (!temp) {
				free(data);
				fclose(inputfile);
				return ERROR_REALLOC;
			}
			data = temp;
			
			fclose(inputfile);
			totalsize += numread;
			f->length = totalsize;
			f->data = data;
			return 0;
		}
		totalsize += FILE_BUFFER_SIZE;
	}
	
	fclose(inputfile);
	
	return 0;
}

int WriteFileData(FileData *f, const char *filename, const char *mode) {
	FILE *outputfile = fopen(filename, mode);
	if (!outputfile) return ERROR_FILE_OPEN;
	
	if (fwrite(f->data, sizeof(unsigned char), f->length, outputfile) != f->length) {
		fclose(outputfile);
		return ERROR_FILE_WRITE;
	} else {
		fclose(outputfile);
		return 0;
	}
}

const char *TranslateError(int error) {
	switch (error) {
		case ERROR_FILE_OPEN:
			return "Error opening file.";
		case ERROR_FILE_READ:
			return "Error reading file.";
		case ERROR_FILE_WRITE:
			return "Error writing file.";
		case ERROR_MALLOC:
		case ERROR_REALLOC:
			return "Error allocating memory.";
		default:
			return "Unknown error.";
	}
}

