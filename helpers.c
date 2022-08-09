#include "shell.h"

char *get_pid(void);
char *get_env_value(char *beginning, int len);
void free_args(char **args, char **front);
void var_rep(char **args, int *exe_ret);

/**
 * free_args - Frees up the memory
 * @args: A double pointer passed to the function.
 * @front: A double pointer to the beginning of args.
 *
 */

void free_args(char **args, char **front)
{
	size_t i;

	for (i = 0; args[i] || args[i + 1]; i++)
		free(args[i]);

	free(front);
}

/**
 * get_pid - Gets the current process ID.
 *
 * Return: The current PID.
 */

char *get_pid(void)
{
	char *buffer;
	size_t i = 0;
	ssize_t file;

	file = open("/proc/self/stat", O_RDONLY);
	if (file == -1)
	{
		perror("Cant read file");
		return (NULL);
	}
	buffer = malloc(120);
	if (buffer == NULL)
	{
		close(file);
		return (NULL);
	}
	read(file, buffer, 120);
	while (buffer[i] != ' ')
		i++;
	buffer[i] = '\0';

	close(file);
	return (buffer);
}

/**
 * get_env_value - Gets the environmental variable.
 * @beginning: The environmental input.
 * @len: The length of the environmental variable to look for in a string.
 *
 * Return: the value of the environmental variable.
 *
 */

char *get_env_value(char *beginning, int len)
{
	char **varAddr;
	char *rep = NULL, *tmp, *var;

	var = malloc(len + 1);
	if (var == NULL)
		return (NULL);
	var[0] = '\0';
	_strncat(var, beginning, len);

	varAddr = _getenv(var);
	free(var);
	if (varAddr)
	{
		tmp = *varAddr;
		while (*tmp != '=')
			tmp++;
		tmp++;
		rep = malloc(_strlen(tmp) + 1);
		if (rep)
			_strcpy(rep, tmp);
	}

	return (rep);
}

/**
 * var_rep - Handles variable replacement.
 * @line: A double pointer holding the commands and arguments.
 * @exe_ret: A pointer to the return value of the last executed command.
 *
 */

void var_rep(char **line, int *exe_ret)
{
	int j = 0, k = 0, len;
	char *rep = NULL, *oldLine = NULL, *new_line;

	oldLine = *line;
	for (; oldLine[j]; j++)
	{
		if (oldLine[j] == '$' && oldLine[j + 1] &&
				oldLine[j + 1] != ' ')
		{
			if (oldLine[j + 1] == '$')
			{
				rep = get_pid();
				k = j + 2;
			}
			else if (oldLine[j + 1] == '?')
			{
				rep = _itoa(*exe_ret);
				k = j + 2;
			}
			else if (oldLine[j + 1])
			{
				/* extract the variable name to search for */
				for (k = j + 1; oldLine[k] &&
						oldLine[k] != '$' &&
						oldLine[k] != ' '; k++)
					;
				len = k - (j + 1);
				rep = get_env_value(&oldLine[j + 1], len);
			}
			new_line = malloc(j + _strlen(rep)
					  + _strlen(&oldLine[k]) + 1);
			if (!line)
				return;
			new_line[0] = '\0';
			_strncat(new_line, oldLine, j);
			if (rep)
			{
				_strcat(new_line, rep);
				free(rep);
				rep = NULL;
			}
			_strcat(new_line, &oldLine[k]);
			free(oldLine);
			*line = new_line;
			oldLine = new_line;
			j = -1;
		}
	}
}
