/* Regression test: fflush(NULL) flushes every output stream (ISO C 7.21.5.2).
 *
 * Before the fix, M2libc's fflush() read stream->bufmode first, so
 * fflush(NULL) dereferenced a null pointer instead of flushing stdout, stderr
 * and every stream opened with fopen() or fdopen(). Reported by the M2libc
 * maintainer on the free(NULL) pull request.
 *
 * The regression is M2libc's, so the test must run against this tree's libc:
 *   m2 -f test/fflush-null.c -o fflush-null    (M2-Mesoplanet with this
 *                                               tree's M2libc; crashes
 *                                               before the fix, exits 0
 *                                               with it)
 *   cc test/fflush-null.c && ./a.out           (host libc: always 0; it
 *                                               shows the shape, not the
 *                                               regression)
 * The exit status is the result: 0 pass, nonzero fail. It writes and then
 * removes fflush-null-a.txt and fflush-null-b.txt in the current directory.
 *
 * The bytes written stay in each writer's buffer until a flush, and M2libc's
 * fopen() reads a whole file when it opens it, so every reader is opened
 * only after fflush(NULL). Two writers make the walk over the open-stream
 * list cover more than one entry; the second round shows the list survived
 * the first walk.
 */
#include <stdio.h>
#include <stdlib.h>

/* 0 when the file holds exactly want, 1 otherwise */
int check(char* name, char* want)
{
	FILE* r;
	int c;
	r = fopen(name, "r");
	if(NULL == r) return 1;
	while(0 != want[0])
	{
		c = fgetc(r);
		if(c != want[0])
		{
			fclose(r);
			return 1;
		}
		want = want + 1;
	}
	c = fgetc(r);
	fclose(r);
	if(EOF != c) return 1;
	return 0;
}

FILE* a;
FILE* b;

/* Close the writers still open, remove both files and return code */
int finish(int code)
{
	if(NULL != a) fclose(a);
	if(NULL != b) fclose(b);
	remove("fflush-null-a.txt");
	remove("fflush-null-b.txt");
	return code;
}

int main(void)
{
	a = NULL;
	b = NULL;

	a = fopen("fflush-null-a.txt", "w");
	if(NULL == a) return finish(2);
	b = fopen("fflush-null-b.txt", "w");
	if(NULL == b) return finish(2);

	fputc('x', a);
	fputs("yz", a);
	fputs("12", b);
	if(0 != fflush(NULL)) return finish(3);
	if(0 != check("fflush-null-a.txt", "xyz")) return finish(4);
	if(0 != check("fflush-null-b.txt", "12")) return finish(5);

	/* Second round: the open-stream list must still hold both writers */
	fputs("w", a);
	fputc('3', b);
	if(0 != fflush(NULL)) return finish(6);
	if(0 != check("fflush-null-a.txt", "xyzw")) return finish(7);
	if(0 != check("fflush-null-b.txt", "123")) return finish(8);

	return finish(0);
}
