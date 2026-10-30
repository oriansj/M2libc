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

#include <stdarg.h>
#include <stddef.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

/* Required constants */
/* For file I/O*/
#define EOF 0xFFFFFFFF
#define BUFSIZ 0x1000

/* For lseek */
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

/* Required variables */
FILE* stdin;
FILE* stdout;
FILE* stderr;
FILE* __list;

void __init_io()
{
	__list = NULL;
	stdin = calloc(1, sizeof(FILE));
	stdin->fd = STDIN_FILENO;
	stdin->bufmode = O_RDONLY;
	stdin->buflen = 1;
	stdin->buffer = calloc(2, sizeof(char));

	stdout = calloc(1, sizeof(FILE));
	stdout->fd = STDOUT_FILENO;
	stdout->bufmode = O_WRONLY;
	stdout->buflen = 512;
	stdout->buffer = calloc(514, sizeof(char));

	stderr = calloc(1, sizeof(FILE));
	stderr->fd = STDERR_FILENO;
	stderr->bufmode = O_WRONLY;
	stderr->buflen = 512;
	stderr->buffer = calloc(514, sizeof(char));
}


/* Flush all IO on exit */
int fflush(FILE* stream);
void __kill_io()
{
	fflush(NULL);
}

/* Standard C functions */
/* Getting */
int read(int fd, char* buf, unsigned count);
int fgetc(FILE* f)
{
	/* Only read on read buffers */
	if(O_WRONLY == f->bufmode) return EOF;

	/* Deal with stdin */
	if(STDIN_FILENO == f->fd)
	{
		f->bufpos = 0;
		int r = read(f->fd, f->buffer, 1);

		/* Catch special case of STDIN gets nothing (AN EOF) */
		if(0 == r) return EOF;
	}

	/* Catch EOF */
	if(f->buflen <= f->bufpos) return EOF;

	/* Deal with standard case */
	int ret = f->buffer[f->bufpos];
	f->bufpos = f->bufpos + 1;

	/* Ensure 0xFF doesn't return EOF */
	return (ret & 0xFF);
}

size_t fread( void* buffer, size_t size, size_t count, FILE* stream )
{
	if(0 == size) return 0;
	if(0 == count) return 0;

	long n = size * count;
	char* p = buffer;
	long i;
	unsigned c;
	for(i = 0; i < n; i = i + 1)
	{
		c = fgetc(stream);
		if(EOF == c) return (i/size);
		p[i] = c;
	}

	return (i/size);
}

int getchar()
{
	return fgetc(stdin);
}


char* fgets(char* str, int count, FILE* stream)
{
	int i = 0;
	int ch;
	if(count <= 0) return NULL;
	while(i < count - 1)
	{
		ch = fgetc(stream);
		if(EOF == ch) {
			/* Return null if EOF is first char read */
			if (i == 0) return NULL;
			break;
		}

		str[i] = ch;
		i = i + 1;

		if('\n' == ch) break;
	}

	str[i] = 0;
	return str;
}

/* Putting */
void fputc(char s, FILE* f)
{
	/* Only write on write buffers */
	if(O_RDONLY == f->bufmode) return;

	/* Add to buffer */
	f->buffer[f->bufpos] = s;
	f->bufpos = f->bufpos + 1;

	/* Flush if full or '\n' */
	if(f->bufpos == f->buflen) fflush(f);
	else if(('\n' == s) && (2 >= f->fd)) fflush(f);
}

size_t fwrite(void const* buffer, size_t size, size_t count, FILE* stream )
{
	long n = size * count;
	if(0 == n) return 0;

	char* p = buffer;
	int c;
	long i;
	for(i=0; i < n; i = i + 1)
	{
		c = p[i];
		fputc(c, stream);
	}

	return (i/size);
}

void putchar(char s)
{
	fputc(s, stdout);
}


int fputs(char const* str, FILE* stream)
{
	while(0 != str[0])
	{
		fputc(str[0], stream);
		str = str + 1;
	}
	return 0;
}


int puts(char const* str)
{
	fputs(str, stdout);
	fputc('\n', stdout);
	return 0;
}


int lseek(int fd, int offset, int whence);
int close(int fd);
/* File management */
FILE* fopen(char const* filename, char const* mode)
{
	int f;
	int size;

	if('w' == mode[0]) f = open(filename, O_WRONLY|O_CREAT|O_TRUNC, 00600);
	else if('a' == mode[0]) f = open(filename, O_WRONLY|O_CREAT|O_APPEND, 00600);
	else f = open(filename, 0, 0); /* Everything else is a read */

	/* Negative numbers are error codes */
	if(0 > f)
	{
		return 0;
	}

	FILE* fi = calloc(1, sizeof(FILE));
	if(NULL == fi)
	{
		close(f);
		return 0;
	}

	if('w' == mode[0] || 'a' == mode[0])
	{
		/* Buffer as much as possible */
		fi->buffer = malloc(BUFSIZ * sizeof(char));
		if(NULL == fi->buffer)
		{
			close(f);
			free(fi);
			return 0;
		}
		fi->buflen = BUFSIZ;
		fi->bufmode = O_WRONLY;
	}
	else
	{
		/* Get enough buffer to read it all */
		size = lseek(f, 0, SEEK_END);
		if(size < 0)
		{
			close(f);
			free(fi);
			return 0;
		}
		fi->buffer = malloc((size + 1) * sizeof(char));
		if(NULL == fi->buffer)
		{
			close(f);
			free(fi);
			return 0;
		}
		fi->buflen = size;
		fi->bufmode = O_RDONLY;

		/* Now read it all */
		lseek(f, 0, SEEK_SET);
		read(f, fi->buffer, size);
	}

	fi->next = __list;
	if(NULL != __list) __list->prev = fi;
	__list = fi;
	fi->fd = f;
	return fi;
}

FILE* fdopen(int fd, char* mode)
{
	FILE* fi = calloc(1, sizeof(FILE));
	if(NULL == fi) return 0;
	int size;

	if('w' == mode[0])
	{
		/* Buffer as much as possible */
		fi->buffer = malloc(BUFSIZ * sizeof(char));
		if(NULL == fi->buffer)
		{
			free(fi);
			return 0;
		}
		fi->buflen = BUFSIZ;
		fi->bufmode = O_WRONLY;
	}
	else
	{
		/* Get enough buffer to read it all */
		size = lseek(fd, 0, SEEK_END);
		if(size < 0)
		{
			free(fi);
			return 0;
		}
		fi->buffer = malloc((size + 1) * sizeof(char));
		if(NULL == fi->buffer)
		{
			free(fi);
			return 0;
		}
		fi->buflen = size;
		fi->bufmode = O_RDONLY;

		/* Now read it all */
		lseek(fd, 0, SEEK_SET);
		read(fd, fi->buffer, size);
	}

	fi->next = __list;
	if(NULL != __list) __list->prev = fi;
	__list = fi;
	fi->fd = fd;
	return fi;
}


int write(int fd, char* buf, unsigned count);
int fflush(FILE* stream)
{
	/* A null pointer flushes every output stream (ISO C 7.21.5.2) */
	if(NULL == stream)
	{
		FILE* i;
		int ret;
		ret = 0;
		/* Skip a null stdout or stderr, which would recurse into this branch */
		if(NULL != stdout)
		{
			if(0 > fflush(stdout)) ret = EOF;
		}
		if(NULL != stderr)
		{
			if(0 > fflush(stderr)) ret = EOF;
		}

		/* Walk a copy so __list itself survives */
		i = __list;
		while(NULL != i)
		{
			if(0 > fflush(i)) ret = EOF;
			i = i->next;
		}
		return ret;
	}

	/* We only need to flush on writes */
	if(O_RDONLY == stream->bufmode) return 0;

	/* If nothing to flush */
	if(0 ==stream->bufpos) return 0;

	/* The actual flushing */
	int error = write(stream->fd, stream->buffer, stream->bufpos);

	/* Keep track of position */
	stream->file_pos = stream->file_pos + stream->bufpos;
	stream->bufpos = 0;

	return error;
}


int fclose(FILE* stream)
{
	/* Deal with STDIN, STDOUT and STDERR */
	/* No close for you */
	if(2 >= stream->fd) return 0;

	/* We only need to flush on writes */
	if(O_WRONLY == stream->bufmode)
	{
		fflush(stream);
	}

	/* Need to keep the File Descriptor for a moment */
	int fd = stream->fd;

	/* Remove from __list */
	if(NULL != stream->prev) stream->prev->next = stream->next;
	if(NULL != stream->next) stream->next->prev = stream->prev;
	/* Deal with special case of first node in __list */
	if (__list == stream) __list = __list->next;

	/* Free up the buffer and struct used for FILE */
	free(stream->buffer);
	free(stream);

	/* Do the actual closing */
	return close(fd);
}


int unlink(char* filename);
/* File Removal */
int remove(char *pathname)
{
	return unlink(pathname);
}


/* File Positioning */
int ungetc(int ch, FILE* stream)
{
	/* Deal with STDIN, STDOUT and STDERR */
	/* No ungetc for you */
	if(2 >= stream->fd) return EOF;

	/* You can't unget on a write stream! */
	if(O_WRONLY == stream->bufmode) return EOF;

	/* Don't underflow */
	if(0 == stream->bufpos) return EOF;

	/* Don't let crap be shoved into read stream */
	if(stream->buffer[stream->bufpos - 1] != ch) return EOF;

	stream->bufpos = stream->bufpos - 1;

	return ch;
}


long ftell(FILE* stream)
{
	/* Deal with STDIN, STDOUT and STDERR */
	/* No ftell for you */
	if(2 >= stream->fd) return 0;

	/* Deal with buffered output */
	if(O_WRONLY == stream->bufmode) return stream->file_pos + stream->bufpos;

	/* Deal with read */
	return stream->bufpos;
}


int fseek(FILE* f, long offset, int whence)
{
	/* Deal with STDIN, STDOUT and STDERR */
	/* No seek and destroy missions */
	if(2 >= f->fd) return 0;

	/* Deal with ugly case */
	if(O_WRONLY == f->bufmode)
	{
		fflush(f);
		return lseek(f->fd, offset, whence);
	}

	/* Deal with read mode */
	int pos;

	if(SEEK_SET == whence)
	{
		pos = offset;
	}
	else if(SEEK_CUR == whence)
	{
		pos = f->bufpos + offset;
	}
	else if(SEEK_END == whence)
	{
		pos = f->buflen + offset;
	}
	else return -1;

	if(pos < 0) return -1;
	if(pos > f->buflen) return -1;

	f->bufpos = pos;
	return pos;
}


void rewind(FILE* f)
{
	fseek(f, 0, SEEK_SET);
}

char* __unsigned_integer_to_string(unsigned long value, int base, int uppercase)
{
	static char buf[60];

	char* digits = "0123456789abcdef";
	if(uppercase)
	{
		digits = "0123456789ABCDEF";
	}

	char* ptr = &buf + 59;
	*ptr = '\0';

	do
	{
		--ptr;
		*ptr = digits[value % base];
		value /= base;
	}
	while(value != 0);

	return ptr;
}

int __vsnprintf_string_offset;
va_list __vsnprintf_ap;
/* vfprintf resumes a conversion that did not fit: it sets
 * __vsnprintf_skip to the bytes of it already written, which the next
 * vsnprintf call produces again without writing them, and vsnprintf sets
 * __vsnprintf_resume to that count when it stops inside a conversion, else
 * to 0. Every vsnprintf call leaves __vsnprintf_skip at 0, so a direct
 * call never skips. */
int __vsnprintf_skip;
int __vsnprintf_resume;
/* The bytes the current conversion produced, written or skipped. */
int __vsnprintf_produced;
/* Set when a byte did not fit. */
int __vsnprintf_cut;

/* Write count copies of c at s[output], as far as n leaves room for the
 * terminator; return the new output. */
int __vsnprintf_repeat(char* s, size_t n, int output, char c, int count)
{
	if(__vsnprintf_cut) return output;
	/* Bytes an earlier chunk wrote are passed over at once. */
	if(__vsnprintf_skip > 0 && count > 0)
	{
		int passed = count;
		if(__vsnprintf_skip < passed) passed = __vsnprintf_skip;
		__vsnprintf_skip = __vsnprintf_skip - passed;
		__vsnprintf_produced = __vsnprintf_produced + passed;
		count = count - passed;
	}
	while(count > 0)
	{
		if(__vsnprintf_skip > 0)
		{
			__vsnprintf_skip = __vsnprintf_skip - 1;
		}
		else if(output < n - 1)
		{
			s[output] = c;
			output = output + 1;
		}
		else
		{
			__vsnprintf_cut = 1;
			return output;
		}
		__vsnprintf_produced = __vsnprintf_produced + 1;
		count = count - 1;
	}
	return output;
}

/* Write str at s[output] up to its terminator, but at most len bytes when
 * len is not negative, as __vsnprintf_repeat. The bytes an
 * earlier chunk wrote are stepped over, not read again. */
int __vsnprintf_copy(char* s, size_t n, int output, char* str, int len)
{
	int j = 0;
	if(__vsnprintf_cut) return output;
	if(__vsnprintf_skip > 0)
	{
		j = __vsnprintf_skip;
		if(len >= 0 && len < j) j = len;
		__vsnprintf_skip = __vsnprintf_skip - j;
		__vsnprintf_produced = __vsnprintf_produced + j;
	}
	while(len < 0 || j < len)
	{
		if(str[j] == '\0') break;
		output = __vsnprintf_repeat(s, n, output, str[j], 1);
		if(__vsnprintf_cut) return output;
		j = j + 1;
	}
	return output;
}

/* The length of str, or of its first max bytes when max is not negative;
 * no byte past those is read, so str needs no terminator then. */
int __vsnprintf_length(char* str, int max)
{
	int len = 0;
	while(max < 0 || len < max)
	{
		if(str[len] == '\0') break;
		len = len + 1;
	}
	return len;
}

/* A field width or precision of decimal digits at format[*i]; past the
 * largest int it stays at that. */
int __vsnprintf_number(const char* format, int* i)
{
	int value = 0;
	int digit;
	while(format[*i] >= '0' && format[*i] <= '9')
	{
		digit = format[*i] - '0';
		if(value > (2147483647 - digit) / 10) value = 2147483647;
		else value = value * 10 + digit;
		*i = *i + 1;
	}
	return value;
}

int vsnprintf(char* s, size_t n, const char* format, va_list arg)
{
	int i = 0;
	int output = 0;
	char* str;

	__vsnprintf_string_offset = 0;
	__vsnprintf_resume = 0;
	__vsnprintf_cut = 0;

	if(0 == n)
	{
		__vsnprintf_skip = 0;
		return 0;
	}
	while(format[i] != '\0' && output < n - 1)
	{
		if(format[i] == '%')
		{
			/* A conversion specification (C 7.21.6.1): flags, field
			 * width, precision, length modifier and conversion. Where
			 * it starts, for vfprintf to resume it. */
			int start = i;
			va_list start_ap = arg;
			int left = 0;
			int zero = 0;
			int plus = 0;
			int space = 0;
			int alternate = 0;
			int width = 0;
			int precision = -1;
			int is_long = 0;
			__vsnprintf_produced = 0;
			++i;

			while(format[i] == '-' || format[i] == '0' || format[i] == '+' || format[i] == ' ' || format[i] == '#')
			{
				if(format[i] == '-') left = 1;
				else if(format[i] == '0') zero = 1;
				else if(format[i] == '+') plus = 1;
				else if(format[i] == ' ') space = 1;
				else alternate = 1;
				++i;
			}

			/* A negative width argument is a '-' flag and its
			 * magnitude; the most negative int is taken as the largest. */
			if(format[i] == '*')
			{
				width = va_arg(arg, int);
				if(width < 0)
				{
					left = 1;
					if(width < 0 - 2147483647) width = 2147483647;
					else width = 0 - width;
				}
				++i;
			}
			else
			{
				width = __vsnprintf_number(format, &i);
			}

			/* A '.' alone is precision 0; a negative argument is none. */
			if(format[i] == '.')
			{
				++i;
				if(format[i] == '*')
				{
					precision = va_arg(arg, int);
					if(precision < 0) precision = -1;
					++i;
				}
				else
				{
					precision = __vsnprintf_number(format, &i);
				}
			}

			/* The length modifier l: long and unsigned long arguments.
			 * Taken only before a conversion it applies to, so "%lc",
			 * "%ls" and a trailing "%l" stay as before. */
			if(format[i] == 'l')
			{
				if(format[i + 1] == 'd' || format[i + 1] == 'i' || format[i + 1] == 'u' || format[i + 1] == 'x' || format[i + 1] == 'X' || format[i + 1] == 'o')
				{
					is_long = 1;
					++i;
				}
			}

			if(format[i] == 's')
			{
				str = va_arg(arg, char*);
				/* The padding needs the length only up to the width, so
				 * a long string is not measured whole, nor again when
				 * vfprintf resumes it. */
				int bound = width;
				if(precision >= 0 && precision < bound) bound = precision;
				int pad = width - __vsnprintf_length(str, bound);
				if(0 == left) output = __vsnprintf_repeat(s, n, output, ' ', pad);
				output = __vsnprintf_copy(s, n, output, str, precision);
				if(left) output = __vsnprintf_repeat(s, n, output, ' ', pad);
			}
			else if(format[i] == 'u' || format[i] == 'x' || format[i] == 'X' || format[i] == 'o' || format[i] == 'd' || format[i] == 'i')
			{
				int uppercase = 0;
				int base = 10;
				char* prefix = "";
				unsigned long magnitude;
				if(format[i] == 'x')
				{
					base = 16;
				}
				else if(format[i] == 'X')
				{
					uppercase = 1;
					base = 16;
				}
				else if(format[i] == 'o')
				{
					base = 8;
				}

				/* '+' and ' ' sign only a signed conversion. */
				if(format[i] == 'd' || format[i] == 'i')
				{
					long value;
					if(is_long)
					{
						value = va_arg(arg, long);
					}
					else
					{
						value = va_arg(arg, int);
					}
					/* The magnitude in unsigned arithmetic: -value
					 * overflows for the most negative value. */
					magnitude = value;
					if(value < 0)
					{
						prefix = "-";
						magnitude = 0 - magnitude;
					}
					else if(plus) prefix = "+";
					else if(space) prefix = " ";
				}
				else if(is_long)
				{
					magnitude = va_arg(arg, unsigned long);
				}
				else
				{
					magnitude = va_arg(arg, unsigned int);
				}

				str = __unsigned_integer_to_string(magnitude, base, uppercase);
				int len = __vsnprintf_length(str, -1);
				/* Precision 0 prints no digit of a zero. */
				if(0 == precision && 0 == magnitude) len = 0;
				/* '#' prefixes a nonzero hexadecimal number. */
				if(alternate && 16 == base && 0 != magnitude)
				{
					prefix = "0x";
					if(uppercase) prefix = "0X";
				}
				int zeros = 0;
				if(precision > len) zeros = precision - len;
				/* '#' raises an octal number's precision so that its
				 * first digit is 0. */
				if(alternate && 8 == base && 0 == zeros)
				{
					if(0 == len || str[0] != '0') zeros = 1;
				}
				int prefix_len = __vsnprintf_length(prefix, -1);
				/* width - zeros cannot overflow, both being at least 0. */
				int pad = 0;
				if(width > zeros) pad = width - zeros - len - prefix_len;
				/* '0' pads with zeros after the sign or prefix, unless
				 * '-' or a precision is given. */
				if(zero && 0 == left && precision < 0 && pad > 0)
				{
					zeros = zeros + pad;
					pad = 0;
				}
				if(0 == left) output = __vsnprintf_repeat(s, n, output, ' ', pad);
				output = __vsnprintf_copy(s, n, output, prefix, prefix_len);
				output = __vsnprintf_repeat(s, n, output, '0', zeros);
				output = __vsnprintf_copy(s, n, output, str, len);
				if(left) output = __vsnprintf_repeat(s, n, output, ' ', pad);
			}
			else if(format[i] == 'c')
			{
				char value = va_arg(arg, char);
				if(0 == left) output = __vsnprintf_repeat(s, n, output, ' ', width - 1);
				output = __vsnprintf_repeat(s, n, output, value, 1);
				if(left) output = __vsnprintf_repeat(s, n, output, ' ', width - 1);
			}
			else if(format[i] == '%')
			{
				output = __vsnprintf_repeat(s, n, output, '%', 1);
			}

			/* A conversion that did not fit: stop at its start, with
			 * what it already wrote, for vfprintf to resume it. */
			if(__vsnprintf_cut)
			{
				__vsnprintf_string_offset = start;
				__vsnprintf_ap = start_ap;
				__vsnprintf_resume = __vsnprintf_produced;
				__vsnprintf_skip = 0;
				s[output] = '\0';
				return output;
			}

			/* A '%' at the end of the format has no conversion to step
			 * over: stop at the terminator. */
			if(format[i] != '\0') ++i;
		}
		else
		{
			if(output < n - 1) s[output++] = format[i];
			i = i + 1;
		}
	}

	__vsnprintf_string_offset = i;
	__vsnprintf_ap = arg;
	__vsnprintf_skip = 0;

	if(output < n)
	{
		/* Null terminator doesn't count */
		s[output] = '\0';
	}

	return output;
}

#define PRINTF_BUFFER_SIZE 4096
/* Add one to always have a null terminator */
char printf_buf[PRINTF_BUFFER_SIZE + 1];
int vfprintf(FILE* stream, char* format, va_list arg)
{
	int output = 0;
	va_list ap = arg;

	__vsnprintf_resume = 0;
	do
	{
		/* Resume a conversion the last chunk cut. */
		__vsnprintf_skip = __vsnprintf_resume;
		/* Written by its length: a "%c" of 0 is a byte like any other. */
		int chunk = vsnprintf(printf_buf, PRINTF_BUFFER_SIZE, format, ap);
		output += chunk;
		format += __vsnprintf_string_offset;
		ap = __vsnprintf_ap;
		fwrite(printf_buf, 1, chunk, stream);
	}
	while(__vsnprintf_string_offset != 0 || __vsnprintf_resume != 0);

	return output;
}
#undef PRINTF_BUFFER_SIZE

int vsprintf(char* s, const char* format, va_list arg)
{
	/* M2-Planet doesn't handle large literals that well. */
	return vsnprintf(s, 2147483647, format, arg);
}

int sprintf(char* s, char* format, ...)
{
	va_list ap;
	va_start(ap, format);
	return vsprintf(s, format, ap);
}

int fprintf(FILE* stream, char* format, ...)
{
	va_list ap;
	va_start(ap, format);
	return vfprintf(stream, format, ap);
}

int printf(char* format, ...)
{
	va_list ap;
	va_start(ap, format);
	return vfprintf(stdout, format, ap);
}

int vprintf(const char * format, va_list arg)
{
	return vfprintf(stdout, format, arg);
}

int snprintf(char* s, size_t n, const char* format, ...)
{
	va_list ap;
	va_start(ap, format);
	return vsnprintf(s, n, format, ap);
}
