/* Regression test: the length modifier l in the printf family (C 7.21.6.1).
 *
 * Before the fix, vsnprintf knew no length modifier: in "%lu" the 'l' matched
 * no conversion and was skipped, the 'u' was copied as text and the argument
 * was never taken, so every later conversion read the wrong one. Found in the
 * kor toolchain, whose `fprintf(stderr, "steps %lu\n", n)` printed "steps u".
 *
 * The regression is M2libc's, so the test must run against this tree's libc:
 *   M2LIBC_PATH=<this tree> M2-Mesoplanet --architecture <arch> \
 *       -f test/printf-length.c -o printf-length && ./printf-length
 *                                (exit 1 before the fix, 0 with it)
 *   cc test/printf-length.c && ./a.out   (host libc: always 0; it shows the
 *                                         shape, not the regression)
 * The exit status is the result: 0 pass, 1 fail. Every value fits in 32 bits,
 * so the test means the same where long is 32 bits wide. It stops at the first
 * mismatch: after a missed argument the later ones are read wrongly too.
 */
#include <stdio.h>
/* stdio.h first, as in test/free-null.c. */
#include <stdlib.h>
#include <string.h>

char* buf;

int check(char* want)
{
	if(0 == strcmp(buf, want)) return 0;
	fputs("printf-length: got \"", stdout);
	fputs(buf, stdout);
	fputs("\", want \"", stdout);
	fputs(want, stdout);
	fputs("\"\n", stdout);
	return 1;
}

int main(void)
{
	unsigned long big = 65535;
	unsigned long huge;
	long negative = -12345678;
	long hex = 305441741;
	unsigned long uhex = 305441741;
	long zero = 0;
	unsigned long uzero = 0;

	buf = malloc(128);
	if(NULL == buf) return 1;
	/* 4294967295 without a literal past 2147483647 */
	big = big * 65536 + 65535;

	sprintf(buf, "%lu", big);
	if(check("4294967295")) return 1;
	sprintf(buf, "%lu|%s", big, "ok");
	if(check("4294967295|ok")) return 1;
	sprintf(buf, "%ld|%d|%s", negative, 7, "ok");
	if(check("-12345678|7|ok")) return 1;
	sprintf(buf, "%li %lx %lX %lo|%s", hex, uhex, uhex, uhex, "ok");
	if(check("305441741 1234abcd 1234ABCD 2215125715|ok")) return 1;
	sprintf(buf, "%ld%lu%d", zero, uzero, 3);
	if(check("003")) return 1;
	/* Where long is wider than 32 bits: 2^40, which an int would cut. */
	if(sizeof(long) > 4)
	{
		huge = 1048576;
		huge = huge * 1048576;
		sprintf(buf, "%lu|%s", huge, "ok");
		if(check("1099511627776|ok")) return 1;
	}
	return 0;
}
