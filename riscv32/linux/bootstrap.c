/* Copyright (C) 2016 Jeremiah Orians
 * Copyright (C) 2021 Andrius Štikonas
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
	asm("rd_a0 rs1_fp !-4 lw"
	    "rd_a1 rs1_fp !-8 lw"
	    "rd_a2 rs1_fp !-12 lw"
	    "rd_a7 !63 addi"
	    "ecall");
}

unsigned write(FILE* f, char* buffer, unsigned count) {
	asm("rd_a0 rs1_fp !-4 lw"
	    "rd_a1 rs1_fp !-8 lw"
	    "rd_a2 rs1_fp !-12 lw"
	    "rd_a7 !64 addi"
	    "ecall");
}

FILE* open(char* name, int flag, int mode)
{
	asm("rd_a0 !-100 addi" /* AT_FDCWD */
	    "rd_a1 rs1_fp !-4 lw"
	    "rd_a2 rs1_fp !-8 lw"
	    "rd_a3 rs1_fp !-12 lw"
	    "rd_a7 !56 addi"   /* openat */
	    "ecall");
}

int close(int fd)
{
	asm("rd_a0 rs1_fp !-4 lw"
	    "rd_a7 !57 addi"    /* close */
	    "ecall");
}

int brk(void *addr)
{
	asm("rd_a0 rs1_fp !-4 lw"
	    "rd_a7 !214 addi"   /* brk */
	    "ecall");
}

void exit(int value)
{
	asm("rd_a0 rs1_fp !-4 lw"
	    "rd_a7 !93 addi"   /* exit */
	    "ecall");
}
