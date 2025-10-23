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
	asm("mov_eax, %3"
		"lea_ecx,[esp+DWORD] %8"
		"mov_ecx,[ecx]"
		"lea_edx,[esp+DWORD] %4"
		"mov_edx,[edx]"
		"lea_ebx,[esp+DWORD] %12"
		"mov_ebx,[ebx]"
		"int !0x80");
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

int close(int fd)
{
	asm("lea_ebx,[esp+DWORD] %4"
	    "mov_ebx,[ebx]"
	    "mov_eax, %6"
	    "int !0x80");
}

int brk(void *addr)
{
	asm("mov_eax,[esp+DWORD] %4"
	    "push_eax"
	    "mov_eax, %45"
	    "pop_ebx"
	    "int !0x80");
}

void exit(int value)
{
	asm("pop_ebx"
	    "pop_ebx"
	    "mov_eax, %1"
	    "int !0x80");
}
