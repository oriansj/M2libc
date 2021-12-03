/* Copyright (C) 2021 Andrius Štikonas
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

#define CLD_EXITED      1       /* child has exited */
#define CLD_KILLED      2       /* child was killed */
#define CLD_DUMPED      3       /* child terminated abnormally */
#define CLD_TRAPPED     4       /* traced child has trapped */
#define CLD_STOPPED     5       /* child has stopped */
#define CLD_CONTINUED   6       /* stopped child has continued */

struct siginfo_t {
	int      si_signo;  /* Signal number */
	int      si_code;   /* Signal code */
	int      si_pid;    /* Sending process ID */
	int      si_uid;    /* Real user ID of sending process */
	void    *si_addr;   /* Address of faulting instruction */
	int      si_status; /* Exit value or signal */
	union {
		int     sigval_int; /* Integer value */
		void   *sigval_ptr; /* Pointer value */
	};
};
