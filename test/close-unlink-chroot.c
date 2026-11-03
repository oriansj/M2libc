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

/* Regression test: close, unlink and chroot pass the kernel their argument.
 *
 * A wrapper that hands the kernel its argument's stack address instead of
 * its value makes close fail with EBADF and free nothing, unlink fail with
 * ENOENT and leave the file, and chroot look up the wrong path.
 *
 * Build it with M2-Planet against this tree's libc and run it. The exit
 * status is the result: 0 pass, nonzero fail. It creates and then removes
 * close-unlink-chroot.txt in the current directory; a broken unlink leaves
 * the file behind.
 */
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define TEST_FILE "close-unlink-chroot.txt"

/* M2libc's wrappers return the kernel's result: 0, or the errno negated */
#define EPERM 1

/* Remove the file and return code */
int finish(int code)
{
	unlink(TEST_FILE);
	return code;
}

int main(void)
{
	int fd;
	int again;
	int r;

	fd = open(TEST_FILE, O_WRONLY | O_CREAT | O_TRUNC, 00600);
	if(0 > fd) return 2;
	if(0 != close(fd)) return finish(3);

	/* open returns the lowest free descriptor, so a closed one comes back */
	again = open(TEST_FILE, O_WRONLY, 0);
	if(again != fd) return finish(4);
	if(0 != close(again)) return finish(5);

	if(0 != unlink(TEST_FILE)) return finish(6);
	if(0 <= open(TEST_FILE, O_RDONLY, 0)) return finish(7);

	/* "/" exists, so chroot succeeds as root and fails with EPERM otherwise */
	r = chroot("/");
	if((0 != r) && (-EPERM != r)) return 8;

	return 0;
}
