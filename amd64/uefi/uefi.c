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

#define PAGE_SIZE 4096
#define USER_STACK_SIZE 8388608
#define EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL 1
#define EFI_FILE_MODE_READ 1
#define EFI_FILE_MODE_WRITE 2
#define EFI_FILE_READ_ONLY 1
#define EFI_ALLOCATE_ANY_PAGES 0
#define EFI_LOADER_DATA 2

#define EFI_SUCCESS 0

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
	long number_table_entries;
	void *configuration_table;
};
struct efi_system_table* _system;

struct efi_guid
{
	unsigned data1;
	unsigned data2;
};
struct efi_guid* EFI_LOADED_IMAGE_PROTOCOL_GUID;
struct efi_guid* EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;

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

long __uefi_2(void*, void*)
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

/* Returns _malloc_ptr and not exit value from AllocatePages call.*/
long __uefi_4(void*, void*, void*, void*)
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

long _allocate_pages(unsigned type, unsigned memory_type, unsigned pages, void* memory)
{
	return __uefi_4(type, memory_type, pages, memory, _system->boot_services->allocate_pages);
}

void _free_pages(void* memory, unsigned pages)
{
	return __uefi_2(memory, pages, _system->boot_services->free_pages);
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

void* malloc(unsigned size);

int _init()
{
	__init_malloc_uefi();

	/* Allocate user stack, UEFI stack is not big enough for compilers */
	__user_stack = malloc(USER_STACK_SIZE) + USER_STACK_SIZE;
}

int _cleanup()
{
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
