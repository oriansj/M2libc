/* Copyright (C) 2026 Novavero AI Inc.
 * This file is part of M2-Planet.
 *
 * M2-Planet is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * M2-Planet is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with M2-Planet.  If not, see <http://www.gnu.org/licenses/>.
 */

/* Regression test: fseek returns 0 on success (ISO C 7.21.9.2).
 *
 * Before the fix, M2libc's fseek returned the new position, so a caller
 * checking the result the standard way saw a failed seek whenever the new
 * position was not 0.
 *
 * Build it with M2-Planet against this tree's libc and run it. The exit
 * status is the result: 0 pass, nonzero fail. It writes and then removes
 * fseek-return.txt in the current directory.
 */
#include <stdio.h>
#include <stdlib.h>

FILE* f;

/* Close the stream if one is open, remove the file and return code */
int finish(int code)
{
	if(NULL != f) fclose(f);
	remove("fseek-return.txt");
	return code;
}

int main(void)
{
	f = fopen("fseek-return.txt", "w");
	if(NULL == f) return finish(2);
	fputs("abcdef", f);
	fclose(f);

	/* A read stream holds the whole file and seeks within it */
	f = fopen("fseek-return.txt", "r");
	if(NULL == f) return finish(3);
	if(0 != fseek(f, 0, SEEK_END)) return finish(4);
	if(0 != fseek(f, 3, SEEK_SET)) return finish(5);
	if('d' != fgetc(f)) return finish(6);
	if(0 != fseek(f, 1, SEEK_CUR)) return finish(7);
	if('f' != fgetc(f)) return finish(8);
	if(0 == fseek(f, -1, SEEK_SET)) return finish(9);
	fclose(f);

	/* A write stream flushes, then seeks with lseek */
	f = fopen("fseek-return.txt", "w");
	if(NULL == f) return finish(10);
	fputs("abcdef", f);
	if(0 != fseek(f, 2, SEEK_SET)) return finish(11);

	return finish(0);
}
