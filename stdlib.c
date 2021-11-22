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

#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0

void exit(int value);

long _malloc_ptr;
long _brk_ptr;

void free(void* l)
{
	return;
}


void* malloc(unsigned size)
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

	return ptr;
}


void* calloc(int count, int size)
{
	void* ret = malloc(count * size);
	if(NULL == ret) return NULL;
	memset(ret, 0, (count * size));
	return ret;
}


/* USED EXCLUSIVELY BY MKSTEMP */
void __set_name(char* s, int i)
{
	s[5] = '0' + (i % 10);
	i = i / 10;
	s[4] = '0' + (i % 10);
	i = i / 10;
	s[3] = '0' + (i % 10);
	i = i / 10;
	s[2] = '0' + (i % 10);
	i = i / 10;
	s[1] = '0' + (i % 10);
	i = i / 10;
	s[0] = '0' + i;
}

int mkstemp(char *template)
{
	int i = 0;
	while(0 != template[i]) i = i + 1;
	i = i - 1;

	/* String MUST be more than 6 characters in length */
	if(i < 6) return -1;

	int count = 6;
	int c;
	while(count > 0)
	{
		c = template[i];
		/* last 6 chars must be X */
		if('X' != c) return -1;
		template[i] = '0';
		i = i - 1;
		count = count - 1;
	}

	int F = -1;
	count = -1;
	while(-1 == F)
	{
		/* Just give up after the planet has blown up */
		if(9000 < count) return -1;

		/* Try upto 9000 unique filenames before stopping */
		count = count + 1;
		__set_name(template+i+1, count);

		/* Pray we can */
		F = open(template, O_CREAT | O_EXCL | O_RDWR | S_IRUSR | S_IWUSR , 0);
	}

	/* well that only took count many tries */
	return F;
}
