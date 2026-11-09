/* Run with M2libc, not the host libc. Creates and removes a temporary file. */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(void)
{
	char* name = "stdio-buffered-position.tmp";
	FILE* w = fopen(name, "w");
	if(NULL == w) return 1;
	int i = 0;
	while(i < 10000)
	{
		fputc(i % 251, w);
		i = i + 1;
	}
	if(fclose(w) != 0) return 2;

	int fd = open(name, O_RDONLY, 0);
	if(fd < 0) return 3;
	if(lseek(fd, -1, SEEK_END) != 9999) return 26;
	int seek_status = lseek(fd, -1, SEEK_SET);
	if(seek_status >= 0) return 27;
	if(lseek(fd, 37, SEEK_SET) != 37) return 4;
	FILE* r = fdopen(fd, "r");
	if(NULL == r) return 5;
	if(ftell(r) != 37) return 6;
	if(fgetc(r) != 37) return 7;
	if(ftell(r) != 38) return 8;
	seek_status = fseek(r, 4095, SEEK_SET);
	if(seek_status < 0) return 9;
	if(fgetc(r) != 4095 % 251) return 10;
	if(ftell(r) != 4096) return 11;
	seek_status = fseek(r, 1, SEEK_CUR);
	if(seek_status < 0) return 12;
	if(fgetc(r) != 4097 % 251) return 13;
	seek_status = fseek(r, -1, SEEK_END);
	if(seek_status < 0) return 14;
	if(fgetc(r) != 9999 % 251) return 15;
	if(fgetc(r) != EOF) return 16;
	seek_status = fseek(r, -1, SEEK_SET);
	if(seek_status >= 0) return 17;
	if(fclose(r) != 0) return 18;

	fd = open(name, O_WRONLY, 0);
	if(fd < 0) return 19;
	if(lseek(fd, 37, SEEK_SET) != 37) return 20;
	w = fdopen(fd, "w");
	if(NULL == w) return 21;
	if(ftell(w) != 37) return 22;
	fputc('X', w);
	if(ftell(w) != 38) return 23;
	if(fclose(w) != 0) return 24;
	if(unlink(name) != 0) return 25;
	return 0;
}
