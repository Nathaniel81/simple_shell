#include "shell.h"

char *err_126(char **args);
char *err_127(char **args);

/**
 * err_126 - Creates an error message for permission denied failures.
 * @args: An array of arguments passed.
 *
 * Return: The error string.
 */

char *err_126(char **args)
{
	int length;
	char *err, *hist_str;

	hist_str = _itoa(hist);
	if (!hist_str)
		return (NULL);

	length = _strlen(name) + _strlen(hist_str) + _strlen(args[0]) + 24;
	err = malloc(sizeof(char) * (length + 1));
	if (err == NULL)
	{
		free(hist_str);
		return (NULL);
	}

	_strcpy(err, name);
	_strcat(err, ": ");
	_strcat(err, hist_str);
	_strcat(err, ": ");
	_strcat(err, args[0]);
	_strcat(err, ": Permission denied\n");

	free(hist_str);
	return (err);
}

/**
 * err_127 - Creates an error message for command not found failures.
 * @args: An array of arguments.
 *
 * Return: The error string.
 */

char *err_127(char **args)
{
	char *err, *hist_str;
	int length;

	hist_str = _itoa(hist);
	if (!hist_str)
		return (NULL);

	length = _strlen(name) + _strlen(hist_str) + _strlen(args[0]) + 16;
	err = malloc(sizeof(char) * (length + 1));
	if (err == NULL)
	{
		free(hist_str);
		return (NULL);
	}

	_strcpy(err, name);
	_strcat(err, ": ");
	_strcat(err, hist_str);
	_strcat(err, ": ");
	_strcat(err, args[0]);
	_strcat(err, ": not found\n");

	free(hist_str);
	return (err);
}
