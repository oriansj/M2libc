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
	asm("!4 R0 SUB R12 ARITH_ALWAYS"
	    "!0 R0 LOAD32 R0 MEMORY"
	    "!8 R1 SUB R12 ARITH_ALWAYS"
	    "!0 R1 LOAD32 R1 MEMORY"
	    "!12 R2 SUB R12 ARITH_ALWAYS"
	    "!0 R2 LOAD32 R2 MEMORY"
	    "!3 R7 LOADI8_ALWAYS"
	    "SYSCALL_ALWAYS");
}

unsigned write(FILE* f, char* buffer, unsigned count) {
	asm("!4 R0 SUB R12 ARITH_ALWAYS"
	    "!0 R0 LOAD32 R0 MEMORY"
	    "!8 R1 SUB R12 ARITH_ALWAYS"
	    "!0 R1 LOAD32 R1 MEMORY"
	    "!12 R2 SUB R12 ARITH_ALWAYS"
	    "!0 R2 LOAD32 R2 MEMORY"
	    "!4 R7 LOADI8_ALWAYS"
	    "SYSCALL_ALWAYS");
}

FILE* open(char* name, int flag, int mode)
{
	asm("!4 R0 SUB R12 ARITH_ALWAYS"
	    "!0 R0 LOAD32 R0 MEMORY"
	    "!8 R1 SUB R12 ARITH_ALWAYS"
	    "!0 R1 LOAD32 R1 MEMORY"
	    "!12 R2 SUB R12 ARITH_ALWAYS"
	    "!0 R2 LOAD32 R2 MEMORY"
	    "!5 R7 LOADI8_ALWAYS"
	    "SYSCALL_ALWAYS");
}

int close(int fd)
{
	asm("!4 R0 SUB R12 ARITH_ALWAYS"
	    "!6 R7 LOADI8_ALWAYS"
	    "SYSCALL_ALWAYS");
}

int brk(void *addr)
{
	asm("!4 R0 SUB R12 ARITH_ALWAYS"
	    "!0 R0 LOAD32 R0 MEMORY"
	    "!45 R7 LOADI8_ALWAYS"
	    "SYSCALL_ALWAYS");
}

void exit(int value)
{
	asm("!4 R0 SUB R12 ARITH_ALWAYS"
	    "!0 R0 LOAD32 R0 MEMORY"
	    "!1 R7 LOADI8_ALWAYS"
	    "SYSCALL_ALWAYS");
}
