/* Copyright (C) 2016 Jeremiah Orians
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

#ifndef __FCNTL_C
#define __FCNTL_C

#include <amd64/uefi/uefi.c>

#define O_RDONLY 0
#define O_WRONLY 1
#define O_RDWR 2
#define O_CREAT 00100
#define O_EXCL 00200
#define O_TRUNC 001000
#define O_APPEND 002000

#define S_IXUSR 00100
#define S_IWUSR 00200
#define S_IRUSR 00400
#define S_IRWXU 00700


int __open(struct efi_file_protocol* _rootdir, char* name, long mode, long attributes)
{
	struct efi_file_protocol* new_handle;
	unsigned rval = __uefi_5(_rootdir, &new_handle, name, mode, attributes, _rootdir->open);
	if(rval != NULL)
	{
	    return -1;
	}
	return new_handle;
}

void free(void* l);

int _open(char* name, int flag, int mode)
{
	int fd;

	char* wide_filename = _posix_path_to_uefi(name);
	if(flag == O_WRONLY|O_CREAT|O_TRUNC)
	{
		long mode = 1 << 63; /* EFI_FILE_MODE_CREATE = 0x8000000000000000 */
		mode = mode | EFI_FILE_MODE_WRITE | EFI_FILE_MODE_READ;
		fd = __open(_rootdir, wide_filename, mode, 0);
	}
	else
	{       /* Everything else is a read */
		fd = __open(_rootdir, wide_filename, EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY);
	}
	free(wide_filename);
	return fd;
}

#define STDIN_FILENO  0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2
#endif
