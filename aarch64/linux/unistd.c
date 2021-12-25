/* Copyright (C) 2020 Jeremiah Orians
 * Copyright (C) 2020 deesix <deesix@tuta.io>
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

#ifndef _UNISTD_C
#define _UNISTD_C
#define NULL 0
#define __PATH_MAX 4096

void* malloc(unsigned size);

int access(char* pathname, int mode)
{
	asm("SET_X0_FROM_BP" "SUB_X0_16" "DEREF_X0"
	    "SET_X2_FROM_X0"
	    "SET_X0_FROM_BP" "SUB_X0_8" "DEREF_X0"
	    "SET_X1_FROM_X0"
	    "SET_X0_TO_0"
	    "SET_X3_FROM_X0"
	    "SET_X0_TO_FCNTL_H_AT_FDCWD"
	    "SET_X8_TO_SYS_FACCESSAT"
	    "SYSCALL");
}

int chdir(char* path)
{
	asm("SET_X0_FROM_BP" "SUB_X0_8" "DEREF_X0"
	    "SET_X8_TO_SYS_CHDIR"
	    "SYSCALL");
}

int fchdir(int fd)
{
	asm("SET_X0_FROM_BP" "SUB_X0_8" "DEREF_X0"
	    "SET_X8_TO_SYS_FCHDIR"
	    "SYSCALL");
}

void _exit(int value);

int fork()
{
	asm("SET_X0_TO_0"
	    "SET_X1_FROM_X0"
	    "SET_X2_FROM_X0"
	    "SET_X3_FROM_X0"
	    "SET_X4_FROM_X0"
	    "SET_X5_FROM_X0"
	    "SET_X6_FROM_X0"
	    "SET_X0_TO_17"
	    "SET_X8_TO_SYS_CLONE"
	    "SYSCALL");
}


int waitpid (int pid, int* status_ptr, int options)
{
	asm("SET_X0_TO_MINUS_1"
	    "SET_X3_FROM_X0"
	    "SET_X0_FROM_BP" "SUB_X0_24" "DEREF_X0"
	    "SET_X2_FROM_X0"
	    "SET_X0_FROM_BP" "SUB_X0_16" "DEREF_X0"
	    "SET_X1_FROM_X0"
	    "SET_X0_FROM_BP" "SUB_X0_8" "DEREF_X0"
	    "SET_X8_TO_SYS_WAIT4"
	    "SYSCALL");
}


int execve(char* file_name, char** argv, char** envp)
{
	asm("SET_X0_FROM_BP" "SUB_X0_24" "DEREF_X0"
	    "SET_X2_FROM_X0"
	    "SET_X0_FROM_BP" "SUB_X0_16" "DEREF_X0"
	    "SET_X1_FROM_X0"
	    "SET_X0_FROM_BP" "SUB_X0_8" "DEREF_X0"
	    "SET_X8_TO_SYS_EXECVE"
	    "SYSCALL");
}

int read(int fd, char* buf, unsigned count)
{
	asm("SET_X0_FROM_BP" "SUB_X0_24" "DEREF_X0"
	    "SET_X2_FROM_X0"
	    "SET_X0_FROM_BP" "SUB_X0_16" "DEREF_X0"
	    "SET_X1_FROM_X0"
	    "SET_X0_FROM_BP" "SUB_X0_8" "DEREF_X0"
	    "SET_X8_TO_SYS_READ"
	    "SYSCALL");
}

int write(int fd, char* buf, unsigned count)
{
	asm("SET_X0_FROM_BP" "SUB_X0_24" "DEREF_X0"
	    "SET_X2_FROM_X0"
	    "SET_X0_FROM_BP" "SUB_X0_16" "DEREF_X0"
	    "SET_X1_FROM_X0"
	    "SET_X0_FROM_BP" "SUB_X0_8" "DEREF_X0"
	    "SET_X8_TO_SYS_WRITE"
	    "SYSCALL");
}

int lseek(int fd, int offset, int whence)
{
	asm("SET_X0_TO_MINUS_1"
	    "SET_X3_FROM_X0"
	    "SET_X0_FROM_BP" "SUB_X0_24" "DEREF_X0"
	    "SET_X2_FROM_X0"
	    "SET_X0_FROM_BP" "SUB_X0_16" "DEREF_X0"
	    "SET_X1_FROM_X0"
	    "SET_X0_FROM_BP" "SUB_X0_8" "DEREF_X0"
	    "SET_X8_TO_SYS_LSEEK"
	    "SYSCALL");
}


int close(int fd)
{
	asm("SET_X0_FROM_BP" "SUB_X0_8" "DEREF_X0"
	    "SET_X8_TO_SYS_CLOSE"
	    "SYSCALL");
}


int unlink (char* filename)
{
	asm("SET_X0_FROM_BP" "SUB_X0_8" "DEREF_X0"
	    "SET_X8_TO_SYS_UNLINK"
	    "SYSCALL");
}


int _getcwd(char* buf, int size)
{
	asm("SET_X0_FROM_BP" "SUB_X0_16" "DEREF_X0"
	    "SET_X1_FROM_X0"
	    "SET_X0_FROM_BP" "SUB_X0_8" "DEREF_X0"
	    "SET_X8_TO_SYS_GETCWD"
	    "SYSCALL");
}


char* getcwd(char* buf, unsigned size)
{
	int c = _getcwd(buf, size);
	if(0 == c) return NULL;
	return buf;
}


char* getwd(char* buf)
{
	return getcwd(buf, __PATH_MAX);
}


char* get_current_dir_name()
{
	return getcwd(malloc(__PATH_MAX), __PATH_MAX);
}


int brk(void *addr)
{
	asm("SET_X0_FROM_BP" "SUB_X0_8" "DEREF_X0"
	    "SET_X8_TO_SYS_BRK"
	    "SYSCALL");
}

struct utsname
{
	char sysname[65];    /* Operating system name (e.g., "Linux") */
	char nodename[65];   /* Name within "some implementation-defined network" */
	char release[65];    /* Operating system release (e.g., "2.6.28") */
	char version[65];    /* Operating system version */
	char machine[65];    /* Hardware identifier */
};

int uname(struct utsname* unameData)
{
	asm("SET_X0_FROM_BP" "SUB_X0_8" "DEREF_X0"
	    "SET_X8_TO_SYS_UNAME"
	    "SYSCALL");
}

#endif
