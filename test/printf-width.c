/* Regression test: flags, field width and precision in the printf family
 * (C 7.21.6.1).
 *
 * Before the fix, vsnprintf knew none of them: in "%02X" the '0' and the '2'
 * matched no conversion and were skipped, so the 'X' was copied as text and
 * the argument was never taken, and every later conversion read the wrong
 * one. Found in the kor toolchain, which printed its "\x%02X" escapes digit
 * by digit with "%c" to get around it.
 *
 * The regression is M2libc's, so the test must run against this tree's libc:
 *   M2LIBC_PATH=<this tree> M2-Mesoplanet --architecture <arch> \
 *       -f test/printf-width.c -o printf-width && ./printf-width
 *                                (exit 1 before the fix, 0 with it)
 *   cc test/printf-width.c && ./a.out   (host libc: always 0; it shows the
 *                                        shape, not the regression)
 * The exit status is the result: 0 pass, 1 fail. It stops at the first
 * mismatch: after a missed argument the later ones are read wrongly too.
 * It also checks that vfprintf, which formats in chunks of 4096 bytes,
 * writes a conversion longer than a chunk whole, through a file
 * printf-width.tmp in the current directory, and that a '%' at the end of
 * a format reads nothing past it.
 */
#include <stdio.h>
/* stdio.h first, as in test/free-null.c. */
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

char* buf;

int check(char* want)
{
	if(0 == strcmp(buf, want)) return 0;
	fputs("printf-width: got \"", stdout);
	fputs(buf, stdout);
	fputs("\", want \"", stdout);
	fputs(want, stdout);
	fputs("\"\n", stdout);
	return 1;
}

/* vsnprintf into a buffer of n bytes. */
int format_n(char* s, size_t n, char* format, ...)
{
	va_list ap;
	int r;
	va_start(ap, format);
	r = vsnprintf(s, n, format, ap);
	va_end(ap);
	return r;
}

/* Whether the file holds the string head, count copies of c, then the
 * string rest, and nothing more. */
int file_is(char* name, char* head, int count, int c, char* rest)
{
	FILE* f = fopen(name, "r");
	int i = 0;
	int got;
	if(NULL == f) return 0;
	while(head[i] != '\0')
	{
		if(fgetc(f) != head[i])
		{
			fclose(f);
			fputs("printf-width: wrong text before a long conversion\n", stdout);
			return 0;
		}
		i = i + 1;
	}
	i = 0;
	while(i < count)
	{
		if(fgetc(f) != c)
		{
			fclose(f);
			fputs("printf-width: a long conversion was cut\n", stdout);
			return 0;
		}
		i = i + 1;
	}
	i = 0;
	while(rest[i] != '\0')
	{
		got = fgetc(f);
		if(got != rest[i])
		{
			fclose(f);
			fputs("printf-width: wrong text after a long conversion\n", stdout);
			return 0;
		}
		i = i + 1;
	}
	got = fgetc(f);
	fclose(f);
	if(EOF != got)
	{
		fputs("printf-width: text past the end\n", stdout);
		return 0;
	}
	return 1;
}

int main(void)
{
	char part[3];
	unsigned long big = 65535;

	buf = malloc(256);
	if(NULL == buf) return 1;
	/* 4294967295 without a literal past 2147483647 */
	big = big * 65536 + 65535;
	/* Three bytes without a terminator: precision must stop the read. */
	part[0] = 'a';
	part[1] = 'b';
	part[2] = 'c';

	sprintf(buf, "%02X|%02X|%02X|%s", 10, 255, 4095, "ok");
	if(check("0A|FF|FFF|ok")) return 1;
	sprintf(buf, "\\u%04x|%04x|%s", 31, 74565, "ok");
	if(check("\\u001f|12345|ok")) return 1;
	sprintf(buf, "%.*s|%.2s|%.0s|%.9s|%s", 3, "headline", "xyz", "gone", "short", "ok");
	if(check("hea|xy||short|ok")) return 1;
	sprintf(buf, "%.*s|%s", 3, part, "ok");
	if(check("abc|ok")) return 1;
	sprintf(buf, "%5d|%-5d|%05d|%-05d|%s", 42, 42, -42, -42, "ok");
	if(check("   42|42   |-0042|-42  |ok")) return 1;
	sprintf(buf, "%+d|%+d|% d|% d|%+ d|%s", 7, -7, 7, -7, 7, "ok");
	if(check("+7|-7| 7|-7|+7|ok")) return 1;
	sprintf(buf, "%.3d|%.0d|%.0x|%5.0d|%05.2d|%.3d|%s", 7, 0, 0, 0, 7, -7, "ok");
	if(check("007|||     |   07|-007|ok")) return 1;
	sprintf(buf, "%#x|%#X|%#x|%#o|%#o|%#.3o|%#5x|%#05x|%s", 255, 255, 0, 8, 0, 8, 10, 10, "ok");
	if(check("0xff|0XFF|0|010|0|010|  0xa|0x00a|ok")) return 1;
	sprintf(buf, "%8.3s|%-8s|%3s|%s", "abcdef", "ab", "abcdef", "ok");
	if(check("     abc|ab      |abcdef|ok")) return 1;
	sprintf(buf, "%-3c|%3c|%c|%s", 'a', 'b', 'c', "ok");
	if(check("a  |  b|c|ok")) return 1;
	sprintf(buf, "%*d|%-*d|%*d|%.*d|%s", 4, 1, 4, 2, -4, 3, -1, 5, "ok");
	if(check("   1|2   |3   |5|ok")) return 1;
	sprintf(buf, "%12lu|%-12lu|%012lX|%s", big, big, big, "ok");
	if(check("  4294967295|4294967295  |0000FFFFFFFF|ok")) return 1;
	sprintf(buf, "%+u|% u|%5u|%-5o|%s", 3, 3, 3, 8, "ok");
	if(check("3|3|    3|10   |ok")) return 1;
	sprintf(buf, "%%|%3d|%%", 9);
	if(check("%|  9|%")) return 1;
	sprintf(buf, "%#.0o|%#.0x|%.0o|%08.3d|%08.3x|%.5x|%s", 0, 0, 0, 7, 31, 31, "ok");
	if(check("0|||     007|     01f|0001f|ok")) return 1;
	sprintf(buf, "%12d|%-12d|%.12d|%s", -2147483647 - 1, -2147483647 - 1, -2147483647 - 1, "ok");
	if(check(" -2147483648|-2147483648 |-002147483648|ok")) return 1;
	sprintf(buf, "%0*.*d|%*.*s|%s", 6, 3, 5, 4, 1, "xyz", "ok");
	if(check("   005|   x|ok")) return 1;
	sprintf(buf, "%*d|%*s|%s", 0, 4, 0, "a", "ok");
	if(check("4|a|ok")) return 1;
	sprintf(buf, "%.*s|%.0s|%s", 0, part, part, "ok");
	if(check("||ok")) return 1;

#ifdef __M2__
	/* M2libc only (C leaves it undefined): a '%' at the end of the format,
	 * and flags with no conversion after them, print nothing and read
	 * nothing past the terminator. */
	sprintf(buf, "ab%");
	if(check("ab")) return 1;
	sprintf(buf, "ab%05");
	if(check("ab")) return 1;
#endif

	/* A small buffer: as much as fits, then the terminator, and nothing
	 * written past n bytes. */
	buf[4] = '#';
	format_n(buf, 4, "%5d", 7);
	if(check("   ")) return 1;
	if(buf[4] != '#') return 1;
	format_n(buf, 4, "%-6s|", "ab");
	if(check("ab ")) return 1;
	if(buf[4] != '#') return 1;

	/* Through vfprintf: a conversion longer than its chunk of 4096 bytes,
	 * at the start of a format and after text, written whole. */
	FILE* out = fopen("printf-width.tmp", "w");
	if(NULL == out) return 1;
	fprintf(out, "%9000d|%s", 7, "ok");
	fclose(out);
	if(0 == file_is("printf-width.tmp", "", 8999, ' ', "7|ok")) return 1;
	out = fopen("printf-width.tmp", "w");
	if(NULL == out) return 1;
	fprintf(out, "ab%-9000c|%d", 'z', 5);
	fclose(out);
	if(0 == file_is("printf-width.tmp", "abz", 8999, ' ', "|5")) return 1;

	/* Two long conversions in a row, the second cut at another offset,
	 * then a string of 9000 bytes and a NUL from "%c", which vfprintf
	 * writes as a byte. */
	out = fopen("printf-width.tmp", "w");
	if(NULL == out) return 1;
	fprintf(out, "%-5000d%5000s|", 1, "");
	fclose(out);
	if(0 == file_is("printf-width.tmp", "1", 9999, ' ', "|")) return 1;
	char* long_text = malloc(9001);
	if(NULL == long_text) return 1;
	memset(long_text, 'y', 9000);
	long_text[9000] = '\0';
	out = fopen("printf-width.tmp", "w");
	if(NULL == out) return 1;
	fprintf(out, "%s%c|%3s", long_text, 0, "ok");
	fclose(out);
	FILE* in = fopen("printf-width.tmp", "r");
	if(NULL == in) return 1;
	int k = 0;
	while(k < 9000)
	{
		if(fgetc(in) != 'y') return 1;
		k = k + 1;
	}
	if(fgetc(in) != 0) return 1;
	if(fgetc(in) != '|') return 1;
	if(fgetc(in) != ' ') return 1;
	if(fgetc(in) != 'o') return 1;
	if(fgetc(in) != 'k') return 1;
	if(fgetc(in) != EOF) return 1;
	fclose(in);
	return 0;
}
