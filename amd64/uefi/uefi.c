/* Copyright (C) 2022 Andrius Štikonas
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

#ifndef __uefi__
#define __uefi__

#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define PAGE_SIZE 4096
#define USER_STACK_SIZE 8388608
#define EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL 1
#define EFI_FILE_MODE_READ 1
#define EFI_FILE_MODE_WRITE 2
#define EFI_FILE_READ_ONLY 1
#define EFI_ALLOCATE_ANY_PAGES 0
#define EFI_LOADER_DATA 2

#define EFI_SUCCESS 0
#define EFI_LOAD_ERROR 1
#define EFI_INVALID_PARAMETER 2
#define EFI_UNSUPPORTED 3
#define EFI_BUFFER_TOO_SMALL 5

#define __PATH_MAX 4096

void* _image_handle;
void* _root_device;
void* __user_stack;

int _argc;
char** _argv;

struct efi_simple_text_output_protocol
{
	void* reset;
	void* output_string;
	void* test_string;
	void* query_mode;
	void* set_mode;
	void* set_attribute;
	void* clear_screen;
	void* set_cursor;
	void* enable_cursor;
	void* mode;
};

struct efi_table_header
{
	unsigned signature;
	unsigned revision_and_header_size;
	unsigned crc32_and_reserved;
};

struct efi_boot_table
{
	struct efi_table_header header;

	/* Task Priority Services */
	void* raise_tpl;
	void* restore_tpl;

	/* Memory Services */
	void* allocate_pages;
	void* free_pages;
	void* get_memory_map;
	void* allocate_pool;
	void* free_pool;

	/* Event & Timer Services */
	void* create_event;
	void* set_timer;
	void* wait_for_event;
	void* signal_event;
	void* close_event;
	void* check_event;

	/* Protocol Handler Services */
	void* install_protocol_interface;
	void* reinstall_protocol_interface;
	void* uninstall_protocol_interface;
	void* handle_protocol;
	void* reserved;
	void* register_protocol_notify;
	void* locate_handle;
	void* locate_device_path;
	void* install_configuration_table;

	/* Image Services */
	void* load_image;
	void* start_image;
	void* exit;
	void* unload_image;
	void* exit_boot_services;

	/* Miscellaneous Services */
	void* get_next_monotonic_count;
	void* stall;
	void* set_watchdog_timer;

	/* DriverSupport Services */
	void* connect_controller;
	void* disconnect_controller;

	/* Open and Close Protocol Services */
	void* open_protocol;
	void* close_protocol;
	void* open_protocol_information;

	/* Library Services */
	void* protocols_per_handle;
	void* locate_handle_buffer;
	void* locate_protocol;
	void* install_multiple_protocol_interfaces;
	void* uninstall_multiple_protocol_interfaces;

	/* 32-bit CRC Services */
	void* copy_mem;
	void* set_mem;
	void* create_event_ex;
};

struct efi_system_table
{
	struct efi_table_header header;

	char* firmware_vendor;
	unsigned firmware_revision;
	void* console_in_handle;
	void* con_in;
	void* console_out_handle;
	struct efi_simple_text_output_protocol* con_out;
	void *standard_error_handle;
	struct efi_simple_text_output_protocol* std_err;
	void *runtime_services;
	struct efi_boot_table* boot_services;
	unsigned number_table_entries;
	void *configuration_table;
};
struct efi_system_table* _system;

struct efi_guid
{
	unsigned data1;
	unsigned data2;
};
struct efi_guid EFI_LOADED_IMAGE_PROTOCOL_GUID;
struct efi_guid EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;
struct efi_guid EFI_FILE_INFO_GUID;

struct efi_loaded_image_protocol
{
	unsigned revision;
	void* parent;
	void* system;

	void* device;
	void* filepath;
	void* reserved;

	/* Image's load options */
	unsigned load_options_size;
	void* load_options;

	/* Location of the image in memory */
	void* image_base;
	unsigned image_size;
	unsigned image_code_type;
	unsigned image_data_type;
	void* unload;
};

struct efi_simple_file_system_protocol
{
	unsigned revision;
	void* open_volume;
};

struct efi_file_protocol
{
	unsigned revision;
	void* open;
	void* close;
	void* delete;
	void* read;
	void* write;
	void* get_position;
	void* set_position;
	void* get_info;
	void* set_info;
	void* flush;
	void* open_ex;
	void* read_ex;
	void* write_ex;
	void* flush_ex;
};
struct efi_file_protocol* _rootdir;

struct efi_time
{
	char year[2];
	char month;
	char day;
	char hour;
	char minute;
	char second;
	char pad1;
	char nanosecond[4];
	char time_zone[2];
	char daylight;
	char pad2;
};

struct efi_file_info
{
	unsigned size;
	unsigned file_size;
	unsigned physical_size;
	struct efi_time create_time;
	struct efi_time last_access_time;
	struct efi_time modifiction_time;
	unsigned attribute;
	char file_name[__PATH_MAX];
};

unsigned __uefi_1(void*, void*, FUNCTION f)
{
	asm("lea_rcx,[rbp+DWORD] %-8"
	    "mov_rcx,[rcx]"
	    "lea_rax,[rbp+DWORD] %-16"
	    "mov_rax,[rax]"
	    "sub_rsp, %8"
	    "call_rax"
	    "add_rsp, %8");
}

unsigned __uefi_2(void*, void*, FUNCTION f)
{
	asm("lea_rcx,[rbp+DWORD] %-8"
	    "mov_rcx,[rcx]"
	    "lea_rdx,[rbp+DWORD] %-16"
	    "mov_rdx,[rdx]"
	    "lea_rax,[rbp+DWORD] %-24"
	    "mov_rax,[rax]"
	    "sub_rsp, %16"
	    "call_rax"
	    "add_rsp, %16");
}

unsigned __uefi_3(void*, void*, void*, FUNCTION f)
{
	asm("lea_rcx,[rbp+DWORD] %-8"
	    "mov_rcx,[rcx]"
	    "lea_rdx,[rbp+DWORD] %-16"
	    "mov_rdx,[rdx]"
	    "lea_r8,[rbp+DWORD] %-24"
	    "mov_r8,[r8]"
	    "lea_rax,[rbp+DWORD] %-32"
	    "mov_rax,[rax]"
	    "sub_rsp, %24"
	    "call_rax"
	    "add_rsp, %24");
}

unsigned __uefi_4(void*, void*, void*, void*, FUNCTION f)
{
	asm("lea_rcx,[rbp+DWORD] %-8"
	    "mov_rcx,[rcx]"
	    "lea_rdx,[rbp+DWORD] %-16"
	    "mov_rdx,[rdx]"
	    "lea_r8,[rbp+DWORD] %-24"
	    "mov_r8,[r8]"
	    "lea_r9,[rbp+DWORD] %-32"
	    "mov_r9,[r9]"
	    "lea_rax,[rbp+DWORD] %-40"
	    "mov_rax,[rax]"
	    "sub_rsp, %32"
	    "call_rax"
	    "add_rsp, %32");
}

unsigned __uefi_5(void*, void*, void*, void*, void*, FUNCTION f)
{
	asm("lea_rcx,[rbp+DWORD] %-8"
	    "mov_rcx,[rcx]"
	    "lea_rdx,[rbp+DWORD] %-16"
	    "mov_rdx,[rdx]"
	    "lea_r8,[rbp+DWORD] %-24"
	    "mov_r8,[r8]"
	    "lea_r9,[rbp+DWORD] %-32"
	    "mov_r9,[r9]"
	    "lea_rax,[rbp+DWORD] %-40"
	    "mov_rax,[rax]"
	    "push_rax"
	    "lea_rax,[rbp+DWORD] %-48"
	    "mov_rax,[rax]"
	    "sub_rsp, %32"
	    "call_rax"
	    "add_rsp, %40");
}

unsigned __uefi_6(void*, void*, void*, void*, void*, void*, FUNCTION f)
{
	asm("lea_rcx,[rbp+DWORD] %-8"
	    "mov_rcx,[rcx]"
	    "lea_rdx,[rbp+DWORD] %-16"
	    "mov_rdx,[rdx]"
	    "lea_r8,[rbp+DWORD] %-24"
	    "mov_r8,[r8]"
	    "lea_r9,[rbp+DWORD] %-32"
	    "mov_r9,[r9]"
	    "lea_rax,[rbp+DWORD] %-48"
	    "mov_rax,[rax]"
	    "push_rax"
	    "lea_rax,[rbp+DWORD] %-40"
	    "mov_rax,[rax]"
	    "push_rax"
	    "lea_rax,[rbp+DWORD] %-56"
	    "mov_rax,[rax]"
	    "sub_rsp, %32"
	    "call_rax"
	    "add_rsp, %48");
}

unsigned _allocate_pages(unsigned type, unsigned memory_type, unsigned pages, void* memory)
{
	return __uefi_4(type, memory_type, pages, memory, _system->boot_services->allocate_pages);
}

void _free_pages(void* memory, unsigned pages)
{
	return __uefi_2(memory, pages, _system->boot_services->free_pages);
}

unsigned _open_protocol(void* handle, struct efi_guid* protocol, void* agent_handle, void** interface, void* controller_handle, long attributes, FUNCTION open_protocol)
{
	return __uefi_6(handle, protocol, agent_handle, interface, controller_handle, attributes, _system->boot_services->open_protocol);
}

unsigned _close_protocol(void* handle, struct efi_guid* protocol, void* agent_handle, void* controller_handle)
{
	return __uefi_4(handle, protocol, agent_handle, controller_handle, _system->boot_services->close_protocol);
}

unsigned _open_volume(struct efi_simple_file_system_protocol* rootfs, struct efi_file_protocol** rootdir)
{
	return __uefi_2(rootfs, rootdir, rootfs->open_volume);
}

unsigned _close(struct efi_file_protocol* file)
{
	return __uefi_1(file, file->close);
}

void exit(unsigned value)
{
	goto FUNCTION__exit;
}

struct _allocated_node
{
	struct _allocated_node *next;
	unsigned pages;
};
struct _allocated_node _allocated_pages;

void __init_malloc();

void __init_malloc_uefi()
{
	__init_malloc();
	_allocated_pages.next = NULL;
}

void _free_allocated_memory()
{
	struct _allocated_node* block = _allocated_pages.next;
	struct _allocated_node* next_block;
	while(block)
	{
		next_block = block->next;
		_free_pages(block, block->pages);
		block = next_block;
	}
}

size_t strlen(char const* str);
void* calloc(int count, int size);

void _posix_path_to_uefi(char *narrow_string)
{
	unsigned length = strlen(narrow_string) + 1;
	char *wide_string = calloc(length, 2);
	unsigned i;
	for(i = 0; i < length; i += 1)
	{
		if(narrow_string[i] == '/')
		{
			wide_string[2 * i] = '\\';
		}
		else
		{
			wide_string[2 * i] = narrow_string[i];
		}
	}
	return wide_string;
}

int isspace(char _c);

void _process_load_options(char* load_options)
{
	/* Determine argc */
	_argc = 1; /* command name */
	char *i = load_options;
	unsigned was_space = 0;
	do
	{
		if(isspace(i[0]))
		{
			if(!was_space)
			{
				_argc += 1;
				was_space = 1;
			}
		}
		else
		{
			was_space = 0;
		}
		i += 1;
	} while(i[0] != 0);

	/* Collect argv */
	_argv = calloc(_argc + 1, sizeof(char*));
	i = load_options;
	unsigned j;
	for(j = 0; j < _argc; j += 1)
	{
		_argv[j] = i;
		do
		{
			i += 1;
		} while(!isspace(i[0]));
		i[0] = 0;
		do
		{
			i += 1;
		} while(isspace(i[0]));
	}
}

void* malloc(unsigned size);
typedef char wchar_t;
size_t wcstombs(char* dest, wchar_t const* src, size_t n);
void __init_io();

void _init()
{
	__init_malloc_uefi();

	/* Allocate user stack, UEFI stack is not big enough for compilers */
	__user_stack = malloc(USER_STACK_SIZE) + USER_STACK_SIZE;

	/* Process command line arguments */
	EFI_LOADED_IMAGE_PROTOCOL_GUID.data1 = (0x11D29562 << 32) + 0x5B1B31A1;
	/* We want to add 0xA0003F8E but M2 treats 32-bit values as negatives, in order to
	 * have the same behaviour on 32-bit systems, so restrict to 31-bit constants */
	EFI_LOADED_IMAGE_PROTOCOL_GUID.data2 = (0x3B7269C9 << 32) + 0x50003F8E + 0x50000000;

	__init_io();
	struct efi_loaded_image_protocol* image;
	_open_protocol(_image_handle, &EFI_LOADED_IMAGE_PROTOCOL_GUID, &image, _image_handle, 0, EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL);
	char* load_options = calloc(image->load_options_size, 1);
	wcstombs(load_options, image->load_options, image->load_options_size);
	_process_load_options(load_options);

	EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID.data1 = (0x11D26459 << 32) + 0x564E5B22 + 0x40000000;
	EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID.data2 = (0x3B7269C9 << 32) + 0x5000398E + 0x50000000;

	_root_device = image->device;
	struct efi_simple_file_system_protocol* rootfs;
	_open_protocol(_root_device, &EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID, &rootfs, _image_handle, 0, EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL);
	_open_volume(rootfs, &_rootdir);

	EFI_FILE_INFO_GUID.data1 = (0x11D26D3F << 32) + 0x09576e92;
	EFI_FILE_INFO_GUID.data2 = (0x3B7269C9 << 32) + 0x5000398E + 0x50000000;
}

void __kill_io();

void _cleanup()
{
	__kill_io();
	_close(_rootdir);
	_close_protocol(_root_device, &EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID, _image_handle, 0);
	_close_protocol(_image_handle, &EFI_LOADED_IMAGE_PROTOCOL_GUID, _image_handle, 0);
	_free_allocated_memory();
}

void* _malloc_uefi(unsigned size)
{
	void* memory_block;
	unsigned pages = (size + sizeof(struct _allocated_node) + PAGE_SIZE - 1) / PAGE_SIZE;
	if(_allocate_pages(EFI_ALLOCATE_ANY_PAGES, EFI_LOADER_DATA, pages, &memory_block) != EFI_SUCCESS)
	{
	    return 0;
	}

	struct _allocated_node *new_node;
	new_node = memory_block;
	new_node->pages = pages;
	new_node->next = _allocated_pages->next;
	_allocated_pages.next = new_node;

	return memory_block + sizeof(struct _allocated_node);
}

#endif
