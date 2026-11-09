/* Run with libc-full.M1 and M2LIBC_AUDIT=present. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
	char* value = getenv("M2LIBC_AUDIT");
	if(NULL == value) return 1;
	if(strcmp(value, "present") != 0) return 2;
	return 0;
}
