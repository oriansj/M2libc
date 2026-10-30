/* Regression test: %d and %i of the most negative value (C 7.21.6.1).
 *
 * Before the fix, vsnprintf printed '-' and negated the value, which
 * overflows for the most negative value: it stayed negative, value % 10 was
 * negative and indexed the digit table before its start, so the digits were
 * garbage. In M2-Planet int is register-sized, so on a 64-bit target the
 * case that fails is -9223372036854775808.
 *
 * The regression is M2libc's, so the test must run against this tree's libc:
 *   M2LIBC_PATH=<this tree> M2-Mesoplanet --architecture <arch> \
 *       -f test/printf-min.c -o printf-min && ./printf-min
 *                                (exit 1 before the fix, 0 with it)
 *   cc test/printf-min.c && ./a.out   (host libc: always 0; it shows the
 *                                      shape, not the regression)
 * With the length modifier l, %ld and %li of the most negative long are
 * checked too, chosen by sizeof(long): where int is narrower than long, the
 * %d cases above cannot reach the overflow.
 * The exit status is the result: 0 pass, 1 fail. It stops at the first
 * mismatch. The minimum is computed as -max - 1, which never overflows.
 */
#include <stdio.h>
/* stdio.h first, as in test/free-null.c. */
#include <stdlib.h>
#include <string.h>

char* buf;

int check(char* want)
{
	if(0 == strcmp(buf, want)) return 0;
	fputs("printf-min: got \"", stdout);
	fputs(buf, stdout);
	fputs("\", want \"", stdout);
	fputs(want, stdout);
	fputs("\"\n", stdout);
	return 1;
}

int main(void)
{
	unsigned int ubits = 1;
	int max;
	int min;
	char* want;
	char* want_ok;
	unsigned long ulbits = 1;
	long lmax;
	long lmin;

	buf = malloc(128);
	want_ok = malloc(128);
	if(NULL == buf || NULL == want_ok) return 1;
	/* the largest int: every bit below the sign bit */
	ubits = ubits << (8 * sizeof(int) - 1);
	max = ubits - 1;
	min = -max - 1;
	want = "-2147483648";
	if(sizeof(int) == 8) want = "-9223372036854775808";

	sprintf(buf, "%d", min);
	if(check(want)) return 1;
	sprintf(buf, "%i", min);
	if(check(want)) return 1;
	strcpy(want_ok, want);
	strcat(want_ok, "|ok");
	sprintf(buf, "%d|%s", min, "ok");
	if(check(want_ok)) return 1;
	sprintf(buf, "%d %d %d %d", max, -1, 0, -max);
	if(sizeof(int) == 8)
	{
		if(check("9223372036854775807 -1 0 -9223372036854775807")) return 1;
	}
	else
	{
		if(check("2147483647 -1 0 -2147483647")) return 1;
	}

	ulbits = ulbits << (8 * sizeof(long) - 1);
	lmax = ulbits - 1;
	lmin = -lmax - 1;
	want = "-2147483648";
	if(sizeof(long) == 8) want = "-9223372036854775808";
	sprintf(buf, "%ld", lmin);
	if(check(want)) return 1;
	strcpy(want_ok, want);
	strcat(want_ok, "|ok");
	sprintf(buf, "%li|%s", lmin, "ok");
	if(check(want_ok)) return 1;
	return 0;
}
