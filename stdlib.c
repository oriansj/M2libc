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

#define _ALLOC_HDR_SIZE sizeof(struct _malloc_node)
#define _MIN_ALLOC_SIZE 32
#define _ALLOC_BLOCK_SIZE 131072

void exit(int value);

struct _malloc_node
{
	struct _malloc_node *next;
	struct _malloc_node *prev;
	size_t size;
};
struct _malloc_node _free_list;

void __init_malloc()
{
	_free_list.next = &_free_list;
	_free_list.prev = &_free_list;
	_free_list.size = 0;
}

void __list_add(struct _malloc_node* n, struct _malloc_node* prev, struct _malloc_node* next)
{
	next->prev = n;
	n->next = next;
	n->prev = prev;
	prev->next = n;
}

void _list_add(struct _malloc_node* n, struct _malloc_node* head)
{
	__list_add(n, head, head->next);
}

void _list_add_tail(struct _malloc_node* n, struct _malloc_node* head)
{
	__list_add(n, head->prev, head);
}

void _list_del(struct _malloc_node* entry)
{
	entry->next->prev = entry->prev;
	entry->prev->next = entry->next;
}

void _malloc_addblock(struct _malloc_node *blk, size_t size)
{
	/* This uses M2 pointer arithmetic which is different from C.
	   pointer + 1 moves the pointer by 1 rather than by sizeof(type). */
	blk->size = size - _ALLOC_HDR_SIZE;
	_list_add(blk, &_free_list);
}

/**
 * When we free, we can take our node and check to see if any memory blocks
 * can be combined into larger blocks.  This will help us fight against
 * memory fragmentation in a simple way.
 */
void _defrag_free_list(void)
{
	struct _malloc_node* block;
	struct _malloc_node* last_block = NULL;

	for(block = _free_list.next; block != &_free_list; block = block->next)
	{
		if(last_block)
		{
			if((last_block + _ALLOC_HDR_SIZE + last_block->size) == block)
			{
				last_block->size += _ALLOC_HDR_SIZE + block->size;
				_list_del(block);
				continue;
			}
		}
		last_block = block;
	}
}

void free(void* ptr)
{
	struct _malloc_node* blk;
	struct _malloc_node* free_blk;

	/* Don't free a NULL pointer */
	if(ptr)
	{
		/* Get corresponding allocation block */
		blk = ptr - _ALLOC_HDR_SIZE;

		/* Let's put it back in the proper spot */
		for(free_blk = _free_list.next; free_blk != &_free_list; free_blk = free_blk->next)
		{
			if(free_blk > blk)
			{
				__list_add(blk, free_blk->prev, free_blk);
				goto blockadded;
			}
		}
		_list_add_tail(blk, &_free_list);

	blockadded:
		/* Let's see if we can combine any memory */
		_defrag_free_list();
	}

	return;
}

void* _malloc_free_list(unsigned size)
{
	void* ptr = NULL;
	struct _malloc_node* blk;

	if(size > 0)
	{
		for(blk = _free_list.next; blk != &_free_list; blk = blk->next)
		{
			if(blk->size >= size)
			{
				ptr = blk + _ALLOC_HDR_SIZE;
				break;
			}
		}
	}

	if(ptr)
	{
		if((blk->size - size) >= _MIN_ALLOC_SIZE)
		{
			struct _malloc_node *new_blk;
			new_blk = ptr + size;
			new_blk->size = blk->size - size - _ALLOC_HDR_SIZE;
			blk->size = size;
			__list_add(new_blk, blk, blk->next);
		}
		_list_del(blk);
	}

	return ptr;
}

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

void* malloc(unsigned size)
{
	void* ptr = _malloc_free_list(size);
	if(ptr)
	{
		return ptr;
	}

	unsigned memory_required = (size / _ALLOC_BLOCK_SIZE + 1) * _ALLOC_BLOCK_SIZE;
	void* blk = _malloc_brk(memory_required);

	_malloc_addblock(blk, memory_required);
	ptr = _malloc_free_list(size);
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

		/* Try upto 9000 unique filenames before stopping */
		count = count + 1;
		__set_name(template+i+1, count);

		/* Pray we can */
		fd = open(template, O_RDWR | O_CREAT | O_EXCL, 00600);
	}

	/* well that only took count many tries */
	return fd;
}
