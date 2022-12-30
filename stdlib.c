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

#define _IN_USE 1
#define _NOT_IN_USE 0

typedef char wchar_t;

void exit(int value);

struct _malloc_node
{
	struct _malloc_node *next;
	void* block;
	size_t size;
	int used;
};

struct _malloc_blob
{
	struct _malloc_blob* next;
	struct _malloc_node* blob;
};

struct _malloc_node* _allocated_list;
struct _malloc_node* _free_list;

/********************************
 * The core POSIX malloc        *
 ********************************/
long _malloc_ptr;
long _brk_ptr;
void* _malloc_brk(unsigned size)
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


/* Setup our initial tracking structure */
void __init_malloc()
{
#ifdef __uefi__
	_free_list = _malloc_uefi(sizeof(struct _malloc_node));
#else
	_free_list = _malloc_brk(sizeof(struct _malloc_node));
#endif
	_free_list->next = NULL;
	_free_list->block = NULL;
	_free_list->size = 0;
	_free_list->used = _IN_USE;
}

void _malloc_insert_block(struct _malloc_node* n, int used)
{
	/* Allocated block doesn't care about order */
	if(_IN_USE == used)
	{
		n->next = _allocated_list;
		_allocated_list = n;
		return;
	}

	/* Free block does */
	struct _malloc_node* i = _free_list;
	struct _malloc_node* last = NULL;
	while(NULL != i)
	{
		/* sort smallest to largest */
		if(n->size < i->size)
		{
			n->next = i;
			if(NULL == last) _free_list = n;
			else last->next = n;
			return;
		}

		/* iterate */
		last = i;
		i = i->next;
	}

	/* Looks like we are the biggest yet */
	last->next = n;
}

/************************************************************************
 * We only mark a block as unused, we don't actually deallocate it here *
 ************************************************************************/
void free(void* ptr)
{
#ifdef _MALLOC_ENABLE_FREE
	struct _malloc_node* i = _allocated_list;
	struct _malloc_node* last = NULL;
	while(NULL != i)
	{
		if(i->block == ptr)
		{
			/* detach */
			if(NULL == last) _allocated_list = i->next;
			else last->next = i->next;

			i->used = _NOT_IN_USE;
			_malloc_insert_block(i, _NOT_IN_USE);
			return;
		}

		/* iterate */
		last = i;
		i = i->next;
	}

	/* we received a pointer to a block that wasn't allocated */
	/* Bail *HARD* */
	exit(EXIT_FAILURE);
#endif
	return;
}

/************************************************************************
 * find if there is any "FREED" blocks big enough to sit on our memory  *
 * budget's face and ruin its life. Respectfully of course              *
 ************************************************************************/
void* _malloc_find_free(unsigned size)
{
	struct _malloc_node* i = _free_list;
	struct _malloc_node* last = NULL;
	while(NULL != i)
	{
		if((_NOT_IN_USE == i->used) && (i->size > size))
		{
			/* disconnect from list */
			if(NULL == last) _free_list = i->next;
			else last->next = i->next;

			/* insert */
			i->used = _IN_USE;
			_malloc_insert_block(i, _IN_USE);
			return i->block;
		}

		/* iterate */
		last = i->next;
		i = i->next;
	}

	/* Couldn't find anything big enough */
	return NULL;
}

void* _malloc_add_new(unsigned size)
{
	struct _malloc_node* n;
#ifdef __uefi__
	n = _malloc_uefi(sizeof(struct _malloc_node));
	n->block = _malloc_uefi(size);
#else
	n = _malloc_brk(sizeof(struct _malloc_node));
	n->block = _malloc_brk(size);
#endif
	n->size = size;
	n->used = _IN_USE;
	_malloc_insert_block(n, _IN_USE);
	return n->block;
}

void* malloc(unsigned size)
{
	void* ptr = _malloc_find_free(size);
	if(NULL == ptr)
	{
		ptr = _malloc_add_new(size);
	}
	return ptr;
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

	int fd = -1;
	count = -1;
	/* open will return -17 or other values */
	while(0 > fd)
	{
		/* Just give up after the planet has blown up */
		if(9000 < count) return -1;

		/* Try up to 9000 unique filenames before stopping */
		count = count + 1;
		__set_name(template+i+1, count);

		/* Pray we can */
		fd = open(template, O_RDWR | O_CREAT | O_EXCL, 00600);
	}

	/* well that only took count many tries */
	return fd;
}

size_t wcstombs(char* dest, char* src, size_t n)
{
	int i = 0;

	do
	{
		dest[i] = src[2 * i];
		if(dest[i] == 0)
		{
			break;
		}
		i = i + 1;
		n = n - 1;
	} while (n != 0);

	return i;
}
