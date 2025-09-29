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

enum
{
	stdin = 0,
	stdout = 1,
	stderr = 2,
};

enum
{
	EOF = 0xFFFFFFFF,
	NULL = 0,
};

enum
{
	EXIT_FAILURE = 1,
	EXIT_SUCCESS = 0,
};

enum
{
	TRUE = 1,
	FALSE = 0,
};

void* malloc(int size);

unsigned read(FILE* f, char* buffer, unsigned count) {
	asm("mov_eax, %3"
		"lea_ecx,[esp+DWORD] %8"
		"mov_ecx,[ecx]"
		"lea_edx,[esp+DWORD] %4"
		"mov_edx,[edx]"
		"lea_ebx,[esp+DWORD] %12"
		"mov_ebx,[ebx]"
		"int !0x80");
}

char* __fputc_buffer;
int fgetc(FILE* f)
{
	/* We don't have operator & */
	if(__fputc_buffer == NULL) {
		__fputc_buffer = malloc(1);
	}

	if(read(f, __fputc_buffer, 1) <= 0) {
		return EOF;
	}

	return __fputc_buffer[0];
}

unsigned fread(char* buffer, unsigned size, unsigned count, FILE* f) {
	return read(f, buffer, size * count);
}


unsigned write(FILE* f, char* buffer, unsigned count) {
	asm("mov_eax, %4"
		"lea_ecx,[esp+DWORD] %8"
		"mov_ecx,[ecx]"
		"lea_edx,[esp+DWORD] %4"
		"mov_edx,[edx]"
		"lea_ebx,[esp+DWORD] %12"
		"mov_ebx,[ebx]"
		"int !0x80");
}

void fputc(char s, FILE* f)
{
	/* We don't have operator & */
	if(__fputc_buffer == NULL) {
		__fputc_buffer = malloc(1);
	}
	__fputc_buffer[0] = s;

	write(f, __fputc_buffer, 1);
}

unsigned fwrite(char* buffer, unsigned size, unsigned count, FILE* f) {
	if(size == 0 || count == 0) {
		return 0;
	}

	return write(f, buffer, size * count);
}

int strlen(char* str )
{
	int i = 0;
	while(0 != str[i]) i = i + 1;
	return i;
}

void fputs(char* s, FILE* f)
{
	write(f, s, strlen(s));
}

FILE* open(char* name, int flag, int mode)
{
	asm("mov_eax, %5"
		"lea_ecx,[esp+DWORD] %8"
		"mov_ecx,[ecx]"
		"lea_edx,[esp+DWORD] %4"
		"mov_edx,[edx]"
		"lea_ebx,[esp+DWORD] %12"
		"mov_ebx,[ebx]"
		"int !0x80");
}

FILE* fopen(char* filename, char* mode)
{
	FILE* f;
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
	return f;
}

int close(int fd)
{
	asm("lea_ebx,[esp+DWORD] %4"
	    "mov_ebx,[ebx]"
	    "mov_eax, %6"
	    "int !0x80");
}

int fclose(FILE* stream)
{
	int error = close(stream);
	return error;
}

int brk(void *addr)
{
	asm("mov_eax,[esp+DWORD] %4"
	    "push_eax"
	    "mov_eax, %45"
	    "pop_ebx"
	    "int !0x80");
}

long _malloc_ptr;
long _brk_ptr;

void* malloc(int size)
{
	if(NULL == _brk_ptr)
	{
		_brk_ptr = brk(0);
		_malloc_ptr = _brk_ptr;
	}

	if(_brk_ptr < _malloc_ptr + size)
	{
		_brk_ptr = brk(_malloc_ptr + size);
		if(-1 == _brk_ptr) return 0;
	}

	long old_malloc = _malloc_ptr;
	_malloc_ptr = _malloc_ptr + size;
	return old_malloc;
}

void* memset(void* ptr, int value, int num)
{
	char* s;
	for(s = ptr; 0 < num; num = num - 1)
	{
		s[0] = value;
		s = s + 1;
	}
}

void* calloc(int count, int size)
{
	void* ret = malloc(count * size);
	if(NULL == ret) return NULL;
	memset(ret, 0, (count * size));
	return ret;
}

void free(void* l)
{
	return;
}

void exit(int value)
{
	asm("pop_ebx"
	    "pop_ebx"
	    "mov_eax, %1"
	    "int !0x80");
}
