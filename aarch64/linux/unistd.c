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
#include <sys/utsname.h>

#define NULL 0
#define __PATH_MAX 4096

void* malloc(unsigned size);

int access(char* pathname, int mode)
{
	asm("mov_x0,x17" "sub_x0,x0,16" "ldr_x0,[x0]"
	    "mov_x2,x0"
	    "mov_x0,x17" "sub_x0,x0,8" "ldr_x0,[x0]"
	    "mov_x1,x0"
	    "mov_x0,0"
	    "mov_x3,x0"
	    "mov_x0,-100"
	    "mov_x8,48"
	    "svc_0");
}

int chdir(char* path)
{
	asm("mov_x0,x17" "sub_x0,x0,8" "ldr_x0,[x0]"
	    "mov_x8,49"
	    "svc_0");
}

int fchdir(int fd)
{
	asm("mov_x0,x17" "sub_x0,x0,8" "ldr_x0,[x0]"
	    "mov_x8,50"
	    "svc_0");
}

void _exit(int value);

int fork()
{
	asm("mov_x0,0"
	    "mov_x1,x0"
	    "mov_x2,x0"
	    "mov_x3,x0"
	    "mov_x4,x0"
	    "mov_x5,x0"
	    "mov_x6,x0"
	    "mov_x0,17"
	    "mov_x8,220"
	    "svc_0");
}


int waitpid (int pid, int* status_ptr, int options)
{
	asm("mov_x0,-1"
	    "mov_x3,x0"
	    "mov_x0,x17" "sub_x0,x0,24" "ldr_x0,[x0]"
	    "mov_x2,x0"
	    "mov_x0,x17" "sub_x0,x0,16" "ldr_x0,[x0]"
	    "mov_x1,x0"
	    "mov_x0,x17" "sub_x0,x0,8" "ldr_x0,[x0]"
	    "mov_x8,260"
	    "svc_0");
}


int execve(char* file_name, char** argv, char** envp)
{
	asm("mov_x0,x17" "sub_x0,x0,24" "ldr_x0,[x0]"
	    "mov_x2,x0"
	    "mov_x0,x17" "sub_x0,x0,16" "ldr_x0,[x0]"
	    "mov_x1,x0"
	    "mov_x0,x17" "sub_x0,x0,8" "ldr_x0,[x0]"
	    "mov_x8,221"
	    "svc_0");
}

int read(int fd, char* buf, unsigned count)
{
	asm("mov_x0,x17" "sub_x0,x0,24" "ldr_x0,[x0]"
	    "mov_x2,x0"
	    "mov_x0,x17" "sub_x0,x0,16" "ldr_x0,[x0]"
	    "mov_x1,x0"
	    "mov_x0,x17" "sub_x0,x0,8" "ldr_x0,[x0]"
	    "mov_x8,63"
	    "svc_0");
}

int write(int fd, char* buf, unsigned count)
{
	asm("mov_x0,x17" "sub_x0,x0,24" "ldr_x0,[x0]"
	    "mov_x2,x0"
	    "mov_x0,x17" "sub_x0,x0,16" "ldr_x0,[x0]"
	    "mov_x1,x0"
	    "mov_x0,x17" "sub_x0,x0,8" "ldr_x0,[x0]"
	    "mov_x8,64"
	    "svc_0");
}

int lseek(int fd, int offset, int whence)
{
	asm("mov_x0,-1"
	    "mov_x3,x0"
	    "mov_x0,x17" "sub_x0,x0,24" "ldr_x0,[x0]"
	    "mov_x2,x0"
	    "mov_x0,x17" "sub_x0,x0,16" "ldr_x0,[x0]"
	    "mov_x1,x0"
	    "mov_x0,x17" "sub_x0,x0,8" "ldr_x0,[x0]"
	    "mov_x8,62"
	    "svc_0");
}


int close(int fd)
{
	asm("mov_x0,x17" "sub_x0,x0,8" "ldr_x0,[x0]"
	    "mov_x8,57"
	    "svc_0");
}


int unlink (char* filename)
{
	asm("mov_x1,x0"
	    "mov_x2,0"
	    "mov_x0,-100"
	    "mov_x8,35"
	    "svc_0");
}

int symlink(char *path1, char *path2)
{
	asm("mov_x0,x17" "sub_x0,x0,16" "ldr_x0,[x0]"
	    "mov_x2,x0"
	    "mov_x1,-100"
	    "mov_x0,x17" "sub_x0,x0,8" "ldr_x0,[x0]"
	    "mov_x8,36"
	    "svc_0");
}

int _getcwd(char* buf, int size)
{
	asm("mov_x0,x17" "sub_x0,x0,16" "ldr_x0,[x0]"
	    "mov_x1,x0"
	    "mov_x0,x17" "sub_x0,x0,8" "ldr_x0,[x0]"
	    "mov_x8,17"
	    "svc_0");
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
	asm("mov_x0,x17" "sub_x0,x0,8" "ldr_x0,[x0]"
	    "mov_x8,214"
	    "svc_0");
}

int uname(struct utsname* unameData)
{
	asm("mov_x0,x17" "sub_x0,x0,8" "ldr_x0,[x0]"
	    "mov_x8,160"
	    "svc_0");
}

int unshare(int flags)
{
	asm("mov_x0,x17" "sub_x0,x0,8" "ldr_x0,[x0]"
	    "mov_x8,97"
	    "svc_0");
}

int geteuid()
{
	asm("mov_x8,175"
	    "svc_0");
}

int getegid()
{
	asm("mov_x8,177"
	    "svc_0");
}

int chroot(char const *path)
{
	asm("mov_x0,x17" "sub_x0,x0,8" "ldr_x0,[x0]"
	    "mov_x8,51"
	    "svc_0");
}

int mount(char const *source, char const *target, char const *filesystemtype, SCM mountflags, void const *data)
{
	asm("mov_x0,x17" "sub_x0,x0,40" "ldr_x0,[x0]"
	    "mov_x4,x0"
	    "mov_x0,x17" "sub_x0,x0,32" "ldr_x0,[x0]"
	    "mov_x3,x0"
	    "mov_x0,x17" "sub_x0,x0,24" "ldr_x0,[x0]"
	    "mov_x2,x0"
	    "mov_x0,x17" "sub_x0,x0,16" "ldr_x0,[x0]"
	    "mov_x1,x0"
	    "mov_x0,x17" "sub_x0,x0,8" "ldr_x0,[x0]"
	    "mov_x8,40"
	    "svc_0");
}

#endif
