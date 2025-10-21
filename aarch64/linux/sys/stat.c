/* Copyright (C) 2020 Jeremiah Orians
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

#ifndef _SYS_STAT_C
#define _SYS_STAT_C
#include <sys/types.h>

#define S_IRWXU 00700
#define S_IXUSR 00100
#define S_IWUSR 00200
#define S_IRUSR 00400

#define S_ISUID 04000
#define S_ISGID 02000
#define S_IXGRP 00010
#define S_IXOTH 00001
#define S_IRGRP 00040
#define S_IROTH 00004
#define S_IWGRP 00020
#define S_IWOTH 00002
#define S_IRWXG 00070
#define S_IRWXO 00007


int chmod(char *pathname, int mode)
{
	asm("mov_x0,x17" "sub_x0,x0,16" "ldr_x0,[x0]"
	    "mov_x2,x0"
	    "mov_x0,x17" "sub_x0,x0,8" "ldr_x0,[x0]"
	    "mov_x1,x0"
	    "mov_x0,0"
	    "mov_x3,x0"
	    "mov_x0,-100"
	    "mov_x8,53"
	    "svc_0");
}


int fchmod(int a, mode_t b)
{
	asm("mov_x0,x17" "sub_x0,x0,16" "ldr_x0,[x0]"
	    "mov_x2,x0"
	    "mov_x0,x17" "sub_x0,x0,8" "ldr_x0,[x0]"
	    "mov_x1,x0"
	    "mov_x0,0"
	    "mov_x3,x0"
	    "mov_x0,-100"
	    "mov_x8,52"
	    "svc_0");
}


int mkdir(char const* a, mode_t b)
{
	asm("mov_x0,x17" "sub_x0,x0,16" "ldr_x0,[x0]"
	    "mov_x2,x0"
	    "mov_x0,x17" "sub_x0,x0,8" "ldr_x0,[x0]"
	    "mov_x1,x0"
	    "mov_x0,0"
	    "mov_x3,x0"
	    "mov_x0,-100"
	    "mov_x8,34"
	    "svc_0");
}


int mknod(char const* a, mode_t b, dev_t c)
{
	asm("mov_x3,x2"
	    "mov_x2,x1"
	    "mov_x1,x0"
	    "mov_x0,-100"
	    "mov_x8,33"
	    "svc_0");
}


mode_t umask(mode_t m)
{
	asm("mov_x0,x17" "sub_x0,x0,8" "ldr_x0,[x0]"
	    "mov_x8,166"
	    "svc_0");
}
#endif
