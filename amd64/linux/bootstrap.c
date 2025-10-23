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

unsigned read(FILE* f, char* buffer, unsigned count) {
	asm(
			"mov_rax, %0"
			"lea_rsi,[rsp+DWORD] %16"
			"mov_rsi,[rsi]"
			"lea_rdx,[rsp+DWORD] %8"
			"mov_rdx,[rdx]"
			"lea_rdi,[rsp+DWORD] %24"
			"mov_rdi,[rdi]"
			"syscall");
}

unsigned write(FILE* f, char* buffer, unsigned count) {
	asm(
			"mov_rax, %1"
			"lea_rsi,[rsp+DWORD] %16"
			"mov_rsi,[rsi]"
			"lea_rdx,[rsp+DWORD] %8"
			"mov_rdx,[rdx]"
			"lea_rdi,[rsp+DWORD] %24"
			"mov_rdi,[rdi]"
			"syscall");
}

FILE* open(char* name, int flag, int mode)
{
	asm("lea_rdi,[rsp+DWORD] %24"
	    "mov_rdi,[rdi]"
	    "lea_rsi,[rsp+DWORD] %16"
	    "mov_rsi,[rsi]"
	    "lea_rdx,[rsp+DWORD] %8"
	    "mov_rdx,[rdx]"
	    "mov_rax, %2"
	    "syscall");
}

int close(int fd)
{
	asm("lea_rdi,[rsp+DWORD] %8"
	    "mov_rdi,[rdi]"
	    "mov_rax, %3"
	    "syscall");
}

int brk(void *addr)
{
	asm("mov_rax,[rsp+DWORD] %8"
	    "push_rax"
	    "mov_rax, %12"
	    "pop_rbx"
	    "mov_rdi,rbx"
	    "syscall");
}

void exit(int value)
{
	asm("pop_rbx"
	    "pop_rdi"
	    "mov_rax, %0x3C"
	    "syscall");
}
