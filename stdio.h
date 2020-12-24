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

// CONSTANT stdin 0
// CONSTANT stdout 1
// CONSTANT stderr 2
// CONSTANT EOF 0xFFFFFFFF

struct FILE
{
	int fd;
	int bufmode; /* 0 = empty, 1 = read, 2 = write */
	int bufpos;
	int buflen;
	char* buffer;
};

int fgetc(FILE* f)
{
	struct __fileinfo* fi = __file_lookup_handle(f);
	if (__FILE_BUFMODE_READ == fi->bufmode && fi->bufpos < fi->buflen)
	{
		fi->bufpos = fi->bufpos + 1;
		return fi->buffer[fi->bufpos - 1];
	}
	__file_set_bufmode(fi, __FILE_BUFMODE_READ);
	fi->buflen = syscall_read(fi->fd, fi->buffer, __FILE_BUFFER_SIZE);
	if(0 < fi->buflen)
	{
		fi->bufpos = 1;
		return fi->buffer[0];
	}
	else
	{
		fi->bufpos = 0;
		return EOF;
	}
}

void fputc(char s, FILE* f)
{
	struct __fileinfo* fi = __file_lookup_handle(f);
	__file_set_bufmode(fi, __FILE_BUFMODE_WRITE);
	if (fi->buflen == __FILE_BUFFER_SIZE)
	{
		syscall_write(fi->fd, fi->buffer, fi->buflen);
		fi->buflen = 0;
	}
	fi->buffer[fi->buflen] = s;
	fi->buflen = fi->buflen + 1;
	if (fi->fd < 3)
	{
		/* flush std streams immediately */
		syscall_write(fi->fd, fi->buffer, fi->buflen);
		fi->buflen = 0;
	}
}

/* Important values needed for open */
// CONSTANT O_RDONLY 0
// CONSTANT O_WRONLY 1
// CONSTANT O_RDWR 2
// CONSTANT O_CREAT 64
// CONSTANT O_TRUNC 512
/* 00700 in octal is 448*/
// CONSTANT S_IRWXU 448
/* 00100 in octal is 64 */
// CONSTANT S_IXUSR 64
/* 00200 in octal is 128 */
// CONSTANT S_IWUSR 128
/* 00400 in octal is 256 */
// CONSTANT S_IRUSR 256

FILE* fopen(char* filename, char* mode)
{
	int f;
	if('w' == mode[0])
	{ /* 577 is O_WRONLY|O_CREAT|O_TRUNC, 384 is 600 in octal */
		f = open(filename, 577 , 384);
	}
	else
	{ /* Everything else is a read */
		f = open(filename, 0, 0);
	}

	/* Negative numbers are error codes */
	if(0 > f)
	{
		return 0;
	}
	struct __fileinfo* fi = calloc(1, sizeof(struct __fileinfo));
	fi->buffer = calloc(__FILE_BUFFER_SIZE, sizeof(char));
	fi->fd = f;
	return fi;
}


int fclose(FILE* stream)
{
	struct __fileinfo* fi = __file_lookup_handle(stream);
	__file_set_bufmode(fi, __FILE_BUFMODE_EMPTY);
	int error = syscall_close(fi->fd);
	return error;
}

int fflush(FILE *stream)
{
	struct __fileinfo* fi = __file_lookup_handle(stream);
	__file_set_bufmode(fi, __FILE_BUFMODE_EMPTY);
	return 0;
}

int fseek(FILE* f, long offset, int whence)
{
	struct __fileinfo* fi = __file_lookup_handle(f);
	__file_set_bufmode(fi, __FILE_BUFMODE_EMPTY);
	return syscall_lseek(fi->fd, offset, whence);
}

void rewind(FILE* f)
{
	fseek(f, 0, SEEK_SET);
}
