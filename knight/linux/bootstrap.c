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

unsigned read(FILE* f, char* buf, unsigned count)
{
	asm("LOAD R0 R14 0"
	    "LOAD R1 R14 4"
	    "LOAD R2 R14 8"
	    "SYS_READ");
}

unsigned write(FILE* f, char* buf, unsigned count)
{
	asm("LOAD R0 R14 0"
	    "LOAD R1 R14 4"
	    "LOAD R2 R14 8"
	    "SYS_WRITE");
}

FILE* open(char* name, int flag, int mode)
{
	asm("LOAD R0 R14 0"
	    "LOAD R1 R14 4"
	    "LOAD R2 R14 8"
	    "SYS_OPEN"
	    "FALSE R2");
}

int close(int fd)
{
	asm("LOAD R0 R14 0"
	    "SYS_CLOSE");
}

int brk(void *addr)
{
	asm("LOAD R0 R14 0"
	    "ADDU R0 R12 R0"
	    "SWAP R0 R12");
}

void exit(int value)
{
	asm("LOAD R0 R14 0"
	    "SYS_EXIT");
}
