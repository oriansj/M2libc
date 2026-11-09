/* Run with libc-full.M1 and M2LIBC_AUDIT=present. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
	char* value = getenv("M2LIBC_AUDIT");
	return NULL == value || strcmp(value, "present") != 0;
}
