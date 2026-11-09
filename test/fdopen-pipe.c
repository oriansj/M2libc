/* Pass a pipe containing "abc" as argv[1]'s descriptor, or on stdin. */
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
	int fd = 0;
	if(argc > 1) fd = atoi(argv[1]);
	FILE* r = fdopen(fd, "r");
	if(NULL == r || ftell(r) != -1 || fgetc(r) != 'a') return 1;
	int status = fseek(r, 0, SEEK_CUR);
	if(status >= 0 || ftell(r) != -1) return 2;
	if(fgetc(r) != 'b' || fgetc(r) != 'c' || fgetc(r) != EOF) return 3;
	if(ftell(r) != -1) return 4;
	return fclose(r);
}
