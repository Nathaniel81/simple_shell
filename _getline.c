#include "shell.h"

void *_realloc(void *ptr, unsigned int old_size, unsigned int new_size);
void assign_lineptr(char **lineptr, size_t *n, char *buffer, size_t b);
ssize_t _getline(char **lineptr, size_t *n, FILE *stream);

/**
 * _realloc - Reallocates a memory block.
 * @ptr: A pointer to previously allocated memory.
 * @old_size: The allocated space for ptr's size in bytes.
 * @new_size: The size in bytes for the new memory block.
 *
 * Return: A pointer to the reallocated memory block.
 */

void *_realloc(void *ptr, unsigned int old_size, unsigned int new_size)
{
	char *ptrCopy, *fill;
	unsigned int index;
	void *mem;

	if (new_size == old_size)
		return (ptr);

	if (ptr == NULL)
	{
		mem = malloc(new_size);
		if (mem == NULL)
			return (NULL);

		return (mem);
	}

	if (new_size == 0 && ptr != NULL)
	{
		free(ptr);
		return (NULL);
	}

	ptrCopy = ptr;
	mem = malloc(sizeof(*ptrCopy) * new_size);
	if (mem == NULL)
	{
		free(ptr);
		return (NULL);
	}

	fill = mem;

	for (index = 0; index < old_size && index < new_size; index++)
		fill[index] = *ptrCopy++;

	free(ptr);
	return (mem);
}

/**
 * assign_lineptr - Reassigns the lineptr variable for _getline.
 * @lineptr: A buffer passed to store a string.
 * @n: Size of lineptr.
 * @buffer: The string to be assigned to the lineptr.
 * @b: Size of buffer.
 *
 */

void assign_lineptr(char **lineptr, size_t *n, char *buffer, size_t b)
{
	if (*lineptr == NULL)
	{
		if (b > 120)
			*n = b;
		else
			*n = 120;
		*lineptr = buffer;
	}
	else if (*n < b)
	{
		if (b > 120)
			*n = b;
		else
			*n = 120;
		*lineptr = buffer;
	}
	else
	{
		_strcpy(*lineptr, buffer);
		free(buffer);
	}
}

/**
 * _getline - Reads input from a stream.
 * @lineptr: A buffer to store the input.
 * @n: Size of lineptr.
 * @stream: The stream input.
 *
 * Return: The number of bytes.
 */

ssize_t _getline(char **lineptr, size_t *n, FILE *stream)
{
	static ssize_t input;
	ssize_t ret;
	char c = 'x', *buffer;
	int rd;

	if (input == 0)
		fflush(stream);
	else
		return (-1);
	input = 0;

	buffer = malloc(sizeof(char) * 120);
	if (buffer == NULL)
		return (-1);

	while (c != '\n')
	{
		rd = read(STDIN_FILENO, &c, 1);
		if (rd == -1 || (rd == 0 && input == 0))
		{
			free(buffer);
			return (-1);
		}
		if (rd == 0 && input != 0)
		{
			input++;
			break;
		}

		if (input >= 120)
			buffer = _realloc(buffer, input, input + 1);

		buffer[input] = c;
		input++;
	}
	buffer[input] = '\0';

	assign_lineptr(lineptr, n, buffer, input);

	ret = input;
	if (rd != 0)
		input = 0;
	return (ret);
}
