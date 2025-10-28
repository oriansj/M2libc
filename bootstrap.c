/* Copyright (C) 2016 Jeremiah Orians
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

int fclose(FILE* stream)
{
	int error = close(stream);
	return error;
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

void* memset(void* ptr, int value, int num)
{
	char* s;
	for(s = ptr; 0 < num; num = num - 1)
	{
		s[0] = value;
		s = s + 1;
	}
}

/* The real memcpy has void* parameters and return types, but
 * for M2-Planet it doesn't matter and it leads to slightly better
 * codegen if they're char* from the start. */
char* memcpy(char* dst, char* src, unsigned count)
{
	if(NULL == dst) return dst;
	if(NULL == src) return NULL;

	unsigned i = 0;
	while(i < count)
	{
		dst[i] = src[i];
		i = i + 1;
	}

	return dst;
}

int strcmp(char* lhs, char* rhs)
{
	int i = 0;
	while(0 != lhs[i])
	{
		if(lhs[i] != rhs[i]) return lhs[i] - rhs[i];
		i = i + 1;
	}

	return lhs[i] - rhs[i];
}

char* strchr(char* str, int ch)
{
	char* p = str;
	while(ch != p[0])
	{
		if(0 == p[0]) return NULL;
		p = p + 1;
	}
	if(0 == p[0]) return NULL;
	return p;
}

int isspace(int c) {
	return c == ' ' || c == '\n' || c == '\t';
}

int isdigit(int c) {
	return c >= '0' && c <= '9';
}

int isalpha(int c) {
	c = c | 32; /* lowercase the char */
	return c >= 'a' && c <= 'z';
}

int isalnum(int c) {
	return isalpha(c) || isdigit(c);
}

int atoi(char* str)
{
	while(isspace(str[0]))
	{
		str = str + 1;
	}

	int negative = 0;
	if (str[0] == '-') {
		negative = 1;
	}
	if (str[0] == '+') {
		str = str + 1;
	}

	int value = 0;
	while(isdigit(str[0]))
	{
		value = 10 * value - (str[0] - '0');
		str = str + 1;
	}

	if(negative == 1)
	{
		return value;
	}
	else
	{
		return -value;
	}
}

void* calloc(int count, int size)
{
	void* ret = malloc(count * size);
	if(NULL == ret) return NULL;
	memset(ret, 0, (count * size));
	return ret;
}

void assert(int condition) {
	if(!condition) {
		fputs("M2libc assertion failed\n", stderr);
		exit(EXIT_FAILURE);
	}
}

void free(void* l)
{
	return;
}
