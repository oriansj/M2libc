/* Regression test: free(NULL) takes no action (ISO C 7.22.3.3).
 *
 * Before the fix, M2libc's free() walked its allocation list and answered a
 * pointer it never allocated -- including NULL -- with exit(EXIT_FAILURE), so
 * a conforming caller of free(NULL) died with exit status 1 and no message.
 * Found in the kor toolchain: cfg_match's iset_rehash frees a NULL table on
 * its first use, and the chain-built kor _selftest stopped there.
 *
 * The regression is M2libc's, so the test must run against this tree's libc:
 *   m2 -f test/free-null.c -o free-null    (M2-Mesoplanet with this tree's
 *                                           M2libc; exit 1 before the fix,
 *                                           0 with it)
 *   cc test/free-null.c && ./a.out         (host libc: always 0; it shows
 *                                           the shape, not the regression)
 * The exit status is the result: 0 pass, 1 fail. malloc(0) returning NULL
 * without registering a block is why the guard cannot disturb the allocator's
 * accounting; a free of an unallocated non-NULL pointer keeps bailing.
 */
#include <stdlib.h>

int main(void)
{
	void* p;

	free(NULL);
	free(NULL); /* repeated calls stay legal */
	p = malloc(16);
	if(NULL == p) return 1;
	free(p);
	free(NULL); /* after a real free too */
	return 0;
}
