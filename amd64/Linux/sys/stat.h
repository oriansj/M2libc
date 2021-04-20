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

#include <sys/types.h>

#define S_IRWXU 00700
#define S_IXUSR 00100
#define S_IWUSR 00200
#define S_IRUSR 00400

#define S_ISUID 0400
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
	asm("LOAD_EFFECTIVE_ADDRESS_rdi %16"
	    "LOAD_INTEGER_rdi"
	    "LOAD_EFFECTIVE_ADDRESS_rsi %8"
	    "LOAD_INTEGER_rsi"
	    "LOAD_IMMEDIATE_rax %90"
	    "SYSCALL");
}


int fchmod(int a, mode_t b)
{
	asm("LOAD_EFFECTIVE_ADDRESS_rdi %16"
	    "LOAD_INTEGER_rdi"
	    "LOAD_EFFECTIVE_ADDRESS_rsi %8"
	    "LOAD_INTEGER_rsi"
	    "LOAD_IMMEDIATE_rax %91"
	    "SYSCALL");
}


int mkdir(char const* a, mode_t b)
{
	asm("LOAD_EFFECTIVE_ADDRESS_rdi %16"
	    "LOAD_INTEGER_rdi"
	    "LOAD_EFFECTIVE_ADDRESS_rsi %8"
	    "LOAD_INTEGER_rsi"
	    "LOAD_IMMEDIATE_rax %83"
	    "SYSCALL");
}


int mknod(char const* a, mode_t b, dev_t c)
{
	asm("LOAD_EFFECTIVE_ADDRESS_rdi %24"
	    "LOAD_INTEGER_rdi"
	    "LOAD_EFFECTIVE_ADDRESS_rsi %16"
	    "LOAD_INTEGER_rsi"
	    "LOAD_EFFECTIVE_ADDRESS_rdx %8"
	    "LOAD_INTEGER_rdx"
	    "LOAD_IMMEDIATE_rax %133"
	    "SYSCALL");
}


mode_t umask(mode_t m)
{
	asm("LOAD_EFFECTIVE_ADDRESS_rdi %8"
	    "LOAD_INTEGER_rdi"
	    "LOAD_IMMEDIATE_rax %95"
	    "SYSCALL");
}
