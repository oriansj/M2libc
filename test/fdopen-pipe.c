/* Pass a pipe descriptor containing "abc" as argv[1], or supply it on stdin.
 * Run with M2libc. Nonseekable streams must remain readable without inventing
 * a seek position, including after refills and an unsuccessful fseek. */
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
	int fd = 0;
	if(argc > 1) fd = atoi(argv[1]);
	FILE* r = fdopen(fd, "r");
	if(NULL == r) return 1;
	if(ftell(r) != -1) return 2;
	if(fgetc(r) != 'a') return 3;
	int seek_status = fseek(r, 0, SEEK_CUR);
	if(seek_status >= 0) return 4;
	if(ftell(r) != -1) return 5;
	if(fgetc(r) != 'b') return 6;
	if(fgetc(r) != 'c') return 7;
	if(fgetc(r) != EOF) return 8;
	if(ftell(r) != -1) return 9;
	if(fclose(r) != 0) return 10;
	return 0;
}
