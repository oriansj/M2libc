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

int access(char* pathname, int mode)
{
	asm("LOAD_EFFECTIVE_ADDRESS_ebx %8"
	    "LOAD_INTEGER_ebx"
	    "LOAD_EFFECTIVE_ADDRESS_ecx %4"
	    "LOAD_INTEGER_ecx"
	    "LOAD_IMMEDIATE_eax %33"
	    "INT_80");
}

int chdir(char* path)
{
	asm("LOAD_EFFECTIVE_ADDRESS_ebx %4"
	    "LOAD_INTEGER_ebx"
	    "LOAD_IMMEDIATE_eax %12"
	    "INT_80");
}

int fchdir(int fd)
{
	asm("LOAD_EFFECTIVE_ADDRESS_ebx %4"
	    "LOAD_INTEGER_ebx"
	    "LOAD_IMMEDIATE_eax %133"
	    "INT_80");
}

void _exit(int value)
{
	asm("POP_ebx"
	    "POP_ebx"
	    "LOAD_IMMEDIATE_eax %1"
	    "INT_80");
}


int fork()
{
	asm("LOAD_IMMEDIATE_eax %2"
	    "LOAD_IMMEDIATE_ebx %0"
	    "INT_80");
}


int waitpid (int pid, int* status_ptr, int options)
{
	asm("LOAD_EFFECTIVE_ADDRESS_ebx %12"
	    "LOAD_INTEGER_ebx"
	    "LOAD_EFFECTIVE_ADDRESS_ecx %8"
	    "LOAD_INTEGER_ecx"
	    "LOAD_EFFECTIVE_ADDRESS_edx %4"
	    "LOAD_INTEGER_edx"
	    "LOAD_IMMEDIATE_eax %7"
	    "INT_80");
}


int execve(char* file_name, char** argv, char** envp)
{
	asm("LOAD_EFFECTIVE_ADDRESS_ebx %12"
	    "LOAD_INTEGER_ebx"
	    "LOAD_EFFECTIVE_ADDRESS_ecx %8"
	    "LOAD_INTEGER_ecx"
	    "LOAD_EFFECTIVE_ADDRESS_edx %4"
	    "LOAD_INTEGER_edx"
	    "LOAD_IMMEDIATE_eax %11"
	    "INT_80");
}

int read(int fd, char* buf, unsigned count) {
	asm("LOAD_EFFECTIVE_ADDRESS_ebx %12"
	    "LOAD_INTEGER_ebx"
	    "LOAD_EFFECTIVE_ADDRESS_ecx %8"
	    "LOAD_INTEGER_ecx"
	    "LOAD_EFFECTIVE_ADDRESS_edx %4"
	    "LOAD_INTEGER_edx"
	    "LOAD_IMMEDIATE_eax %3"
	    "INT_80");
}

int write(int fd, char* buf, unsigned count) {
	asm("LOAD_EFFECTIVE_ADDRESS_ebx %12"
	    "LOAD_INTEGER_ebx"
	    "LOAD_EFFECTIVE_ADDRESS_ecx %8"
	    "LOAD_INTEGER_ecx"
	    "LOAD_EFFECTIVE_ADDRESS_edx %4"
	    "LOAD_INTEGER_edx"
	    "LOAD_IMMEDIATE_eax %4"
	    "INT_80");
}

int lseek(int fd, int offset, int whence)
{
	asm("LOAD_EFFECTIVE_ADDRESS_ebx %12"
	    "LOAD_INTEGER_ebx"
	    "LOAD_EFFECTIVE_ADDRESS_ecx %8"
	    "LOAD_INTEGER_ecx"
	    "LOAD_EFFECTIVE_ADDRESS_edx %4"
	    "LOAD_INTEGER_edx"
	    "LOAD_IMMEDIATE_eax %19"
	    "INT_80");
}

int close(int fd)
{
	asm("LOAD_EFFECTIVE_ADDRESS_ebx %4"
	    "LOAD_INTEGER_ebx"
	    "LOAD_IMMEDIATE_eax %6"
	    "INT_80");
}

int _getcwd(char* buf, size_t size)
{
	asm("LOAD_EFFECTIVE_ADDRESS_ebx %8"
	    "LOAD_INTEGER_ebx"
	    "LOAD_EFFECTIVE_ADDRESS_ecx %4"
	    "LOAD_INTEGER_ecx"
	    "LOAD_IMMEDIATE_eax %183"
	    "INT_80");
}

int brk(void *addr)
{
	asm("LOAD_ESP_IMMEDIATE_into_eax %4"
	    "PUSH_eax"
	    "LOAD_IMMEDIATE_eax %45"
	    "POP_ebx"
	    "INT_80");
}

int chmod(char *pathname, int mode)
{
	asm("LOAD_EFFECTIVE_ADDRESS_ebx %8"
	    "LOAD_INTEGER_ebx"
	    "LOAD_EFFECTIVE_ADDRESS_ecx %4"
	    "LOAD_INTEGER_ecx"
	    "LOAD_IMMEDIATE_eax %15"
	    "INT_80");
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
	asm("LOAD_EFFECTIVE_ADDRESS_ebx %4"
	    "LOAD_INTEGER_ebx"
	    "LOAD_IMMEDIATE_eax %109"
	    "INT_80");
}
