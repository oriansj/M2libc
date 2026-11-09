/* Run against M2libc; creates and removes a temporary file. */
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(void)
{
	char* name = "stdio-buffered-position.tmp";
	FILE* f = fopen(name, "w");
	if(NULL == f) return 1;
	int i;
	for(i = 0; i < 10000; i = i + 1) fputc(i % 251, f);
	if(fclose(f) != 0) return 2;
	int fd = open(name, O_RDONLY, 0);
	if(fd < 0 || lseek(fd, -1, SEEK_END) != 9999) return 3;
	int status = lseek(fd, -1, SEEK_SET);
	if(status >= 0 || lseek(fd, 37, SEEK_SET) != 37) return 4;
	f = fdopen(fd, "r");
	if(NULL == f || ftell(f) != 37 || fgetc(f) != 37 || ftell(f) != 38) return 5;
	status = fseek(f, 4095, SEEK_SET);
	if(status < 0 || fgetc(f) != 4095 % 251 || ftell(f) != 4096) return 6;
	status = fseek(f, 1, SEEK_CUR);
	if(status < 0 || fgetc(f) != 4097 % 251) return 7;
	status = fseek(f, -1, SEEK_END);
	if(status < 0 || fgetc(f) != 9999 % 251 || fgetc(f) != EOF) return 8;
	status = fseek(f, -1, SEEK_SET);
	if(status >= 0 || fclose(f) != 0) return 9;
	fd = open(name, O_WRONLY, 0);
	if(fd < 0 || lseek(fd, 37, SEEK_SET) != 37) return 10;
	f = fdopen(fd, "w");
	if(NULL == f || ftell(f) != 37) return 11;
	fputc('X', f);
	if(ftell(f) != 38 || fclose(f) != 0 || unlink(name) != 0) return 12;
	return 0;
}
