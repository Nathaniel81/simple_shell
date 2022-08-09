#include "shell.h"

int len_tok(char *str, char *delim);
char **_strtok(char *line, char *delim);
int tok_counter(char *str, char *delim);

/**
 * len_tok - Finds the delimiter index
 * that marks the end of the first token in a string.
 * @str: The string to look for.
 * @delim: The delimiter character.
 *
 * Return: The initial token's delimiter index pointed to str.
 */

int len_tok(char *str, char *delim)
{
	int index = 0, length = 0;

	while (str[index] && str[index] != *delim)
	{
		length++;
		index++;
	}

	return (length);
}

/**
 * tok_counter - Counts the number of delimited words.
 * @str: The string passed
 * @delim: The delimiter character.
 *
 * Return: The total number of words in str.
 */

int tok_counter(char *str, char *delim)
{
	int index = 0, tokens = 0, length = 0;

	for (; str[index]; index++)
		length++;

	for (index = 0; index < length; index++)
	{
		if (*(str + index) != *delim)
		{
			tokens++;
			index += len_tok(str + index, delim);
		}
	}
	return (tokens);
}

/**
 * _strtok - Tokenizes a string.
 * @line: The string input.
 * @delim: The delimiter character.
 *
 * Return: A pointer to an array containing the tokenized words.
 */

char **_strtok(char *line, char *delim)
{
	char **ptr;
	int index = 0, tokens, t, letters, l;

	tokens = tok_counter(line, delim);
	if (tokens == 0)
		return (NULL);

	ptr = malloc(sizeof(char *) * (tokens + 2));
	if (!ptr)
		return (NULL);

	for (t = 0; t < tokens; t++)
	{
		while (line[index] == *delim)
			index++;

		letters = len_tok(line + index, delim);

		ptr[t] = malloc(sizeof(char) * (letters + 1));
		if (!ptr[t])
		{
			for (index -= 1; index >= 0; index--)
				free(ptr[index]);
			free(ptr);
			return (NULL);
		}

		for (l = 0; l < letters; l++)
		{
			ptr[t][l] = line[index];
			index++;
		}

		ptr[t][l] = '\0';
	}
	ptr[t] = NULL;
	ptr[t + 1] = NULL;

	return (ptr);
}
