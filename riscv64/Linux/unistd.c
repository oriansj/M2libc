/* Copyright (C) 2020 Jeremiah Orians
 * Copyright (C) 2021 Andrius Štikonas
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
#define __SI_SWAP_ERRNO_CODE

void* malloc(unsigned size);

int access(char* pathname, int mode)
{
	asm("RD_A0 !-100 ADDI" /* AT_FDCWD */
	    "RD_A1 RS1_FP !-8 LD"
	    "RD_A2 RS1_FP !-16 LD"
	    "RD_A3 ADDI" /* flags = 0 */
	    "RD_A7 !48 ADDI"
	    "ECALL");
}

int chdir(char* path)
{
	asm("RD_A0 RS1_FP !-8 LD"
	    "RD_A7 !49 ADDI"
	    "ECALL");
}

int fchdir(int fd)
{
	asm("RD_A0 RS1_FP !-8 LD"
	    "RD_A7 !50 ADDI"
	    "ECALL");
}

void _exit(int value);

int fork()
{
	asm("RD_A7 !220 ADDI"
	    "RD_A0 !17 ADDI" /* SIGCHLD */
	    "RD_A1 MV"       /* Child uses duplicate of parent's stack */
	    "ECALL");
}


int waitpid (int pid, int* status_ptr, int options)
{
	/* Uses wait4 with struct rusage *ru set to NULL */
	asm("RD_A0 RS1_FP !-8 LD"
	    "RD_A1 RS1_FP !-16 LD"
	    "RD_A2 RS1_FP !-24 LD"
	    "RD_A3 ADDI"
	    "RD_A7 !260 ADDI"
	    "ECALL");
}


int execve(char* file_name, char** argv, char** envp)
{
	asm("RD_A0 RS1_FP !-8 LD"
	    "RD_A1 RS1_FP !-16 LD"
	    "RD_A2 RS1_FP !-24 LD"
	    "RD_A7 !221 ADDI"
	    "ECALL");
}

int read(int fd, char* buf, unsigned count)
{
	asm("RD_A0 RS1_FP !-8 LD"
	    "RD_A1 RS1_FP !-16 LD"
	    "RD_A2 RS1_FP !-24 LD"
	    "RD_A7 !63 ADDI"
	    "ECALL");
}

int write(int fd, char* buf, unsigned count)
{
	asm("RD_A0 RS1_FP !-8 LD"
	    "RD_A1 RS1_FP !-16 LD"
	    "RD_A2 RS1_FP !-24 LD"
	    "RD_A7 !64 ADDI"
	    "ECALL");
}

int lseek(int fd, int offset, int whence)
{
	asm("RD_A0 RS1_FP !-8 LD"
	    "RD_A1 RS1_FP !-16 LD"
	    "RD_A2 RS1_FP !-24 LD"
	    "RD_A7 !62 ADDI"
	    "ECALL");
}


int close(int fd)
{
	asm("RD_A0 RS1_FP !-8 LD"
	    "RD_A7 !57 ADDI"    /* close */
	    "ECALL");
}


int unlink (char* filename)
{
	asm("RD_A0 !-100 ADDI" /* AT_FDCWD */
	    "RD_A1 RS1_FP !-8 LD"
	    "RD_A2 !0 ADDI"     /* No flags */
	    "RD_A7 !35 ADDI"    /* unlinkat */
	    "ECALL");
}


int _getcwd(char* buf, int size)
{
	asm("RD_A0 RS1_FP !-8 LD"
	    "RD_A1 RS1_FP !-16 LD"
	    "RD_A7 !17 ADDI"
	    "ECALL");
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
	asm("RD_A0 RS1_FP !-8 LD"
	    "RD_A7 !214 ADDI"
	    "ECALL");
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
	asm("RD_A0 RS1_FP !-8 LD"
	    "RD_A7 !160 ADDI"
	    "ECALL");
}

#endif
