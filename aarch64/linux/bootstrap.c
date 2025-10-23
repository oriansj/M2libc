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

unsigned read(FILE* f, char* buffer, unsigned count) {
	asm("mov_x0,x17" "sub_x0,x0,24" "ldr_x0,[x0]"
	    "mov_x2,x0"
	    "mov_x0,x17" "sub_x0,x0,16" "ldr_x0,[x0]"
	    "mov_x1,x0"
	    "mov_x0,x17" "sub_x0,x0,8" "ldr_x0,[x0]"
	    "mov_x8,63"
	    "svc_0");
}

unsigned write(FILE* f, char* buffer, unsigned count) {
	asm("mov_x0,x17" "sub_x0,x0,24" "ldr_x0,[x0]"
	    "mov_x2,x0"
	    "mov_x0,x17" "sub_x0,x0,16" "ldr_x0,[x0]"
	    "mov_x1,x0"
	    "mov_x0,x17" "sub_x0,x0,8" "ldr_x0,[x0]"
	    "mov_x8,64"
	    "svc_0");
}

FILE* open(char* name, int flag, int mode)
{
	asm("mov_x0,x17" "sub_x0,x0,24" "ldr_x0,[x0]"
	    "mov_x3,x0"
	    "mov_x0,x17" "sub_x0,x0,16" "ldr_x0,[x0]"
	    "mov_x2,x0"
	    "mov_x0,x17" "sub_x0,x0,8" "ldr_x0,[x0]"
	    "mov_x1,x0"
	    "mov_x0,-100"
	    "mov_x8,56"
	    "svc_0");
}

int close(int fd)
{
	asm("mov_x0,x17" "sub_x0,x0,8" "ldr_x0,[x0]"
	    "mov_x8,57"
	    "svc_0");
}

int brk(void *addr)
{
	asm("mov_x0,x17" "sub_x0,x0,8" "ldr_x0,[x0]"
	    "mov_x8,214"
	    "svc_0");
}

void exit(int value)
{
	asm("mov_x0,x17" "sub_x0,x0,8" "ldr_x0,[x0]"
	    "mov_x8,93"
	    "svc_0");
}
