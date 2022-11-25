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


#ifndef _UNISTD_C
#define _UNISTD_C

#include <amd64/uefi/uefi.c>

#define NULL 0
#define EOF 0xFFFFFFFF

/* For lseek */
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

void* malloc(unsigned size);

int access(char* pathname, int mode)
{
	asm("lea_rdi,[rsp+DWORD] %16"
	    "mov_rdi,[rdi]"
	    "lea_rsi,[rsp+DWORD] %8"
	    "mov_rsi,[rsi]"
	    "mov_rax, %21"
	    "syscall");
}

int chdir(char* path)
{
	asm("lea_rdi,[rsp+DWORD] %8"
	    "mov_rdi,[rdi]"
	    "mov_rax, %80"
	    "syscall");
}

int fchdir(int fd)
{
	asm("lea_rdi,[rsp+DWORD] %8"
	    "mov_rdi,[rdi]"
	    "mov_rax, %81"
	    "syscall");
}

void _exit(int value);

int fork()
{
	return -1;
}


int waitpid (int pid, int* status_ptr, int options)
{
	/* Uses wait4 with struct rusage *ru set to NULL */
	asm("lea_rdi,[rsp+DWORD] %24"
	    "mov_rdi,[rdi]"
	    "lea_rsi,[rsp+DWORD] %16"
	    "mov_rsi,[rsi]"
	    "lea_rdx,[rsp+DWORD] %8"
	    "mov_rdx,[rdx]"
	    "mov_r10, %0"
	    "mov_rax, %61"
	    "syscall");
}


int execve(char* file_name, char** argv, char** envp)
{
	asm("lea_rdi,[rsp+DWORD] %24"
	    "mov_rdi,[rdi]"
	    "lea_rsi,[rsp+DWORD] %16"
	    "mov_rsi,[rsi]"
	    "lea_rdx,[rsp+DWORD] %8"
	    "mov_rdx,[rdx]"
	    "mov_rax, %59"
	    "syscall");
}

int read(int fd, char* buf, unsigned count)
{
	struct efi_file_protocol* f = fd;
	__uefi_3(fd, &count, buf, f->read);
	return count;
}

int write(int fd, char* buf, unsigned count)
{
	struct efi_file_protocol* f = fd;
	unsigned i;
	char c = 0;

	/* In UEFI StdErr might not be printing stuff to console, so just use stdout */
	if(f == STDOUT_FILENO || f == STDERR_FILENO)
	{
		for(i = 0; i < count; i += 1)
		{
			c = buf[i];
			__uefi_2(_system->con_out, &c, _system->con_out->output_string);
			if('\n' == c)
			{
				c = '\r';
				__uefi_2(_system->con_out, &c, _system->con_out->output_string);
			}
		}
		return i;
	}

	/* Otherwise write to file */
	__uefi_3(f, &count, buf, f->write);
	return count;
}

int _get_file_size(struct efi_file_protocol* f)
{
	/* Preallocate some extra space for file_name */
	size_t file_info_size = sizeof(struct efi_file_info);
	struct efi_file_info* file_info = calloc(1, file_info_size);
	unsigned rval = __uefi_4(f, &EFI_FILE_INFO_GUID, &file_info_size, file_info, f->get_info);
	if(rval != EFI_SUCCESS)
	{
		return -1;
	}
	int file_size = file_info->file_size;
	free(file_info);
	return file_size;
}

int lseek(int fd, int offset, int whence)
{
	struct efi_file_protocol* f = fd;
	if(whence == SEEK_SET)
	{
	}
	else if(whence == SEEK_CUR)
	{
		unsigned position;
		__uefi_2(f, &position, f->get_position);
		offset += position;
	}
	else if(whence == SEEK_END)
	{
		offset += _get_file_size(fd);
	}
	else
	{
		return -1;
	}

	unsigned rval = __uefi_2(f, offset, f->set_position);
	if(rval == EFI_SUCCESS)
	{
		return offset;
	}
	return -1;
}


int close(int fd)
{
	struct efi_file_protocol* f = fd;
	unsigned rval = __uefi_1(f, f->close);
	if(rval != EFI_SUCCESS)
	{
	    return -1;
	}
	return rval;
}


int unlink (char* filename)
{
	asm("lea_rdi,[rsp+DWORD] %8"
	    "mov_rdi,[rdi]"
	    "mov_rax, %87"
	    "syscall");
}


int _getcwd(char* buf, int size)
{
	asm("lea_rdi,[rsp+DWORD] %16"
	    "mov_rdi,[rdi]"
	    "lea_rsi,[rsp+DWORD] %8"
	    "mov_rsi,[rsi]"
	    "mov_rax, %79"
	    "syscall");
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
	return -1;
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
	unameData->sysname = "UEFI";
	unameData->release = "1.0";
	unameData->release = "1.0";
	unameData->machine= "x86_64";
}

#endif
