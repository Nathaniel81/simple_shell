#include "shell.h"

ssize_t get_new_len(char *line);
void handle_line(char **line, ssize_t read);
void logical_ops(char *line, ssize_t *new_len);

/**
 * handle_line - Divides a line read from standard input.
 * @line: A pointer to a line read from standard input.
 * @read: The length of the line.
 *
 */

void handle_line(char **line, ssize_t read)
{
	char *oldLine, *new_line;
	char prv, current, nxt;
	ssize_t newLen;
	size_t i, j;

	newLen = get_new_len(*line);
	if (newLen == read - 1)
		return;
	new_line = malloc(newLen + 1);
	if (new_line == NULL)
		return;
	j = 0;
	oldLine = *line;
	for (i = 0; oldLine[i]; i++)
	{
		current = oldLine[i];
		nxt = oldLine[i + 1];
		if (i != 0)
		{
			prv = oldLine[i - 1];
			if (current == ';')
			{
				if (nxt == ';' && prv != ' ' && prv != ';')
				{
					new_line[j++] = ' ';
					new_line[j++] = ';';
					continue;
				}
				else if (prv == ';' && nxt != ' ')
				{
					new_line[j++] = ';';
					new_line[j++] = ' ';
					continue;
				}
				if (prv != ' ')
					new_line[j++] = ' ';
				new_line[j++] = ';';
				if (nxt != ' ')
					new_line[j++] = ' ';
				continue;
			}
			else if (current == '&')
			{
				if (nxt == '&' && prv != ' ')
					new_line[j++] = ' ';
				else if (prv == '&' && nxt != ' ')
				{
					new_line[j++] = '&';
					new_line[j++] = ' ';
					continue;
				}
			}
			else if (current == '|')
			{
				if (nxt == '|' && prv != ' ')
					new_line[j++]  = ' ';
				else if (prv == '|' && nxt != ' ')
				{
					new_line[j++] = '|';
					new_line[j++] = ' ';
					continue;
				}
			}
		}
		else if (current == ';')
		{
			if (i != 0 && oldLine[i - 1] != ' ')
				new_line[j++] = ' ';
			new_line[j++] = ';';
			if (nxt != ' ' && nxt != ';')
				new_line[j++] = ' ';
			continue;
		}
		new_line[j++] = oldLine[i];
	}
	new_line[j] = '\0';

	free(*line);
	*line = new_line;
}

/**
 * get_new_len - Gets the new length of a line partitioned
 * @line: The line to be checked.
 *
 * Return: The new length of the line.
 *
 */

ssize_t get_new_len(char *line)
{
	size_t i;
	ssize_t newLen = 0;
	char current, nxt;

	for (i = 0; line[i]; i++)
	{
		current = line[i];
		nxt = line[i + 1];
		if (current == '#')
		{
			if (i == 0 || line[i - 1] == ' ')
			{
				line[i] = '\0';
				break;
			}
		}
		else if (i != 0)
		{
			if (current == ';')
			{
				if (nxt == ';' && line[i - 1] != ' ' && line[i - 1] != ';')
				{
					newLen += 2;
					continue;
				}
				else if (line[i - 1] == ';' && nxt != ' ')
				{
					newLen += 2;
					continue;
				}
				if (line[i - 1] != ' ')
					newLen++;
				if (nxt != ' ')
					newLen++;
			}
			else
				logical_ops(&line[i], &newLen);
		}
		else if (current == ';')
		{
			if (i != 0 && line[i - 1] != ' ')
				newLen++;
			if (nxt != ' ' && nxt != ';')
				newLen++;
		}
		newLen++;
	}
	return (newLen);
}
/**
 * logical_ops - Looks for logical operators ("||" or "&&") in a line.
 * @line: A pointer to the character.
 * @new_len: Pointer to new_len.
 *
 */

void logical_ops(char *line, ssize_t *new_len)
{
	char prv, current, nxt;

	prv = *(line - 1);
	current = *line;
	nxt = *(line + 1);

	if (current == '&')
	{
		if (nxt == '&' && prv != ' ')
			(*new_len)++;
		else if (prv == '&' && nxt != ' ')
			(*new_len)++;
	}
	else if (current == '|')
	{
		if (nxt == '|' && prv != ' ')
			(*new_len)++;
		else if (prv == '|' && nxt != ' ')
			(*new_len)++;
	}
}
