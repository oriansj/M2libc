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

#define NULL 0

int access(char* pathname, int mode)
{
	asm("LOAD_EFFECTIVE_ADDRESS_rdi %16"
	    "LOAD_INTEGER_rdi"
	    "LOAD_EFFECTIVE_ADDRESS_rsi %8"
	    "LOAD_INTEGER_rsi"
	    "LOAD_IMMEDIATE_rax %21"
	    "SYSCALL");
}

int chdir(char* path)
{
	asm("LOAD_EFFECTIVE_ADDRESS_rdi %8"
	    "LOAD_INTEGER_rdi"
	    "LOAD_IMMEDIATE_rax %80"
	    "SYSCALL");
}

int fchdir(int fd)
{
	asm("LOAD_EFFECTIVE_ADDRESS_rdi %8"
	    "LOAD_INTEGER_rdi"
	    "LOAD_IMMEDIATE_rax %81"
	    "SYSCALL");
}

void _exit(int value)
{
	asm("POP_RBX"
	    "POP_RDI"
	    "LOAD_IMMEDIATE_rax %0x3C"
	    "SYSCALL");
}


int fork()
{
	asm("LOAD_IMMEDIATE_rax %57"
	    "LOAD_IMMEDIATE_rdi %0"
	    "SYSCALL");
}


int waitpid (int pid, int* status_ptr, int options)
{
	/* Uses wait4 with struct rusage *ru set to NULL */
	asm("LOAD_EFFECTIVE_ADDRESS_rdi %24"
	    "LOAD_INTEGER_rdi"
	    "LOAD_EFFECTIVE_ADDRESS_rsi %16"
	    "LOAD_INTEGER_rsi"
	    "LOAD_EFFECTIVE_ADDRESS_rdx %8"
	    "LOAD_INTEGER_rdx"
	    "LOAD_IMMEDIATE_r10 %0"
	    "LOAD_IMMEDIATE_rax %61"
	    "SYSCALL");
}


int execve(char* file_name, char** argv, char** envp)
{
	asm("LOAD_EFFECTIVE_ADDRESS_rdi %24"
	    "LOAD_INTEGER_rdi"
	    "LOAD_EFFECTIVE_ADDRESS_rsi %16"
	    "LOAD_INTEGER_rsi"
	    "LOAD_EFFECTIVE_ADDRESS_rdx %8"
	    "LOAD_INTEGER_rdx"
	    "LOAD_IMMEDIATE_rax %59"
	    "SYSCALL");
}

int read(int fd, char* buf, unsigned count)
{ /*maybe*/
	asm("LOAD_EFFECTIVE_ADDRESS_rdi %24"
	    "LOAD_INTEGER_rdi"
	    "LOAD_EFFECTIVE_ADDRESS_rsi %16"
	    "LOAD_INTEGER_rsi"
	    "LOAD_EFFECTIVE_ADDRESS_rdx %8"
	    "LOAD_INTEGER_rdx"
	    "LOAD_IMMEDIATE_rax %0"
	    "SYSCALL");
}

int write(int fd, char* buf, unsigned count)
{/*maybe*/
	asm("LOAD_EFFECTIVE_ADDRESS_rdi %24"
	    "LOAD_INTEGER_rdi"
	    "LOAD_EFFECTIVE_ADDRESS_rsi %16"
	    "LOAD_INTEGER_rsi"
	    "LOAD_EFFECTIVE_ADDRESS_rdx %8"
	    "LOAD_INTEGER_rdx"
	    "LOAD_IMMEDIATE_rax %1"
	    "SYSCALL");
}

int lseek(int fd, int offset, int whence)
{
	asm("LOAD_EFFECTIVE_ADDRESS_rdi %24"
	    "LOAD_INTEGER_rdi"
	    "LOAD_EFFECTIVE_ADDRESS_rsi %16"
	    "LOAD_INTEGER_rsi"
	    "LOAD_EFFECTIVE_ADDRESS_rdx %8"
	    "LOAD_INTEGER_rdx"
	    "LOAD_IMMEDIATE_rax %8"
	    "SYSCALL");
}

int close(int fd)
{
	asm("LOAD_EFFECTIVE_ADDRESS_rdi %8"
	    "LOAD_INTEGER_rdi"
	    "LOAD_IMMEDIATE_rax %3"
	    "SYSCALL");
}

int _getcwd(char* buf, int size)
{
	asm("LOAD_EFFECTIVE_ADDRESS_rdi %16"
	    "LOAD_INTEGER_rdi"
	    "LOAD_EFFECTIVE_ADDRESS_rsi %8"
	    "LOAD_INTEGER_rsi"
	    "LOAD_IMMEDIATE_rax %79"
	    "SYSCALL");
}

int brk(void *addr)
{
	asm("LOAD_RSP_IMMEDIATE_into_rax %8"
	    "PUSH_RAX"
	    "LOAD_IMMEDIATE_rax %12"
	    "POP_RBX"
	    "COPY_rbx_to_rdi"
	    "SYSCALL");
}

int chmod(char *pathname, int mode)
{
	asm("LOAD_EFFECTIVE_ADDRESS_rdi %16"
	    "LOAD_INTEGER_rdi"
	    "LOAD_EFFECTIVE_ADDRESS_rsi %8"
	    "LOAD_INTEGER_rsi"
	    "LOAD_IMMEDIATE_rax %90"
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
	asm("LOAD_EFFECTIVE_ADDRESS_rdi %8"
	    "LOAD_INTEGER_rdi"
	    "LOAD_IMMEDIATE_rax %63"
	    "SYSCALL");
}
