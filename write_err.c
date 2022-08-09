#include "shell.h"

char *err_1(char **args);
char *err_2_cd(char **args);
char *err_2_exit(char **args);
char *err_2_syn(char **args);
char *err_env(char **args);

/**
 * err_env - Creates an error message.
 * @args: An array of arguments.
 *
 * Return: The error string.
 */

char *err_env(char **args)
{
	char *err, *hist_str;
	int length;

	hist_str = _itoa(hist);
	if (!hist_str)
		return (NULL);

	args--;
	length = _strlen(name) + _strlen(hist_str) + _strlen(args[0]) + 45;
	err = malloc(sizeof(char) * (length + 1));
	if (!err)
	{
		free(hist_str);
		return (NULL);
	}

	_strcpy(err, name);
	_strcat(err, ": ");
	_strcat(err, hist_str);
	_strcat(err, ": ");
	_strcat(err, args[0]);
	_strcat(err, ": Unable to add/remove from environment\n");

	free(hist_str);
	return (err);
}

/**
 * err_1 - Creates an error message for _alias errors.
 * @args: An array of arguments.
 *
 * Return: The error string.
 */

char *err_1(char **args)
{
	char *err;
	int length;

	length = _strlen(name) + _strlen(args[0]) + 13;
	err = malloc(sizeof(char) * (length + 1));
	if (!err)
		return (NULL);

	_strcpy(err, "alias: ");
	_strcat(err, args[0]);
	_strcat(err, " not found\n");

	return (err);
}

/**
 * err_2_exit - Creates an error message for _b_exit errors.
 * @args: An array of arguments.
 *
 * Return: The error string.
 */

char *err_2_exit(char **args)
{
	char *err, *hist_str;
	int length;

	hist_str = _itoa(hist);
	if (!hist_str)
		return (NULL);

	length = _strlen(name) + _strlen(hist_str) + _strlen(args[0]) + 27;
	err = malloc(sizeof(char) * (length + 1));
	if (!err)
	{
		free(hist_str);
		return (NULL);
	}

	_strcpy(err, name);
	_strcat(err, ": ");
	_strcat(err, hist_str);
	_strcat(err, ": exit: Illegal number: ");
	_strcat(err, args[0]);
	_strcat(err, "\n");

	free(hist_str);
	return (err);
}

/**
 * err_2_cd - Creates an error message for _cd errors.
 * @args: An array of arguments.
 *
 * Return: The error string.
 */

char *err_2_cd(char **args)
{
	char *err, *hist_str;
	int length;

	hist_str = _itoa(hist);
	if (!hist_str)
		return (NULL);

	if (args[0][0] == '-')
		args[0][2] = '\0';
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
	if (args[0][0] == '-')
		_strcat(err, ": cd: Illegal option ");
	else
		_strcat(err, ": cd: can't cd to ");
	_strcat(err, args[0]);
	_strcat(err, "\n");

	free(hist_str);
	return (err);
}

/**
 * err_2_syn - Creates an error message for syntax errors.
 * @args: An array of arguments passed.
 *
 * Return: The error string.
 */

char *err_2_syn(char **args)
{
	char *err, *hist_str;
	int length;

	hist_str = _itoa(hist);
	if (!hist_str)
		return (NULL);

	length = _strlen(name) + _strlen(hist_str) + _strlen(args[0]) + 33;
	err = malloc(sizeof(char) * (length + 1));
	if (!err)
	{
		free(hist_str);
		return (NULL);
	}

	_strcpy(err, name);
	_strcat(err, ": ");
	_strcat(err, hist_str);
	_strcat(err, ": Syntax error: \"");
	_strcat(err, args[0]);
	_strcat(err, "\" unexpected\n");

	free(hist_str);
	return (err);
}
