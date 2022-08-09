#include "shell.h"

char *get_cmd_in(char *line, int *exe_ret);
int cmd_call(char **args, char **front, int *exe_ret);
int run_cmd(char **args, char **front, int *exe_ret);
int arg_handler(int *exe_ret);
int arg_checker(char **args);

/**
 * get_cmd_in - Gets a command from standard input.
 * @line: A buffer to store the command.
 * @exe_ret: The return value of the last executed command.
 *
 * Return: A pointer to the stored command.
 */

char *get_cmd_in(char *line, int *exe_ret)
{
	size_t n = 0;
	ssize_t read;
	char *prompt = "#cisfun$ ";

	if (line)
		free(line);

	read = _getline(&line, &n, STDIN_FILENO);
	if (read == -1)
		return (NULL);
	if (read == 1)
	{
		hist++;
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, prompt, 9);
		return (get_cmd_in(line, exe_ret));
	}

	line[read - 1] = '\0';
	var_rep(&line, exe_ret);
	handle_line(&line, read);

	return (line);
}

/**
 * cmd_call - Divides operators from the commands
 * @args: An array of arguments.
 * @front: A double pointer to the beginning of args.
 * @exe_ret: The return value of the parent process
 *
 * Return: The return value of the last executed command.
 */

int cmd_call(char **args, char **front, int *exe_ret)
{
	int rtn, index;

	if (!args[0])
		return (*exe_ret);
	for (index = 0; args[index]; index++)
	{
		if (_strncmp(args[index], "||", 2) == 0)
		{
			free(args[index]);
			args[index] = NULL;
			args = aliase_rep(args);
			rtn = run_cmd(args, front, exe_ret);
			if (*exe_ret != 0)
			{
				args = &args[++index];
				index = 0;
			}
			else
			{
				for (index++; args[index]; index++)
					free(args[index]);
				return (rtn);
			}
		}
		else if (_strncmp(args[index], "&&", 2) == 0)
		{
			free(args[index]);
			args[index] = NULL;
			args = aliase_rep(args);
			rtn = run_cmd(args, front, exe_ret);
			if (*exe_ret == 0)
			{
				args = &args[++index];
				index = 0;
			}
			else
			{
				for (index++; args[index]; index++)
					free(args[index]);
				return (rtn);
			}
		}
	}
	args = aliase_rep(args);
	rtn = run_cmd(args, front, exe_ret);
	return (rtn);
}

/**
 * run_cmd - Calls the execution of a command.
 * @args: An array of arguments.
 * @front: A double pointer to the beginning of args.
 * @exe_ret: The return value of the parent process.
 *
 * Return: The return value of the last executed command.
 */

int run_cmd(char **args, char **front, int *exe_ret)
{
	int rtn, i;
	int (*builtin)(char **args, char **front);

	builtin = get_builtin(args[0]);

	if (builtin)
	{
		rtn = builtin(args + 1, front);
		if (rtn != EXIT)
			*exe_ret = rtn;
	}
	else
	{
		*exe_ret = execute(args, front);
		rtn = *exe_ret;
	}

	hist++;

	for (i = 0; args[i]; i++)
		free(args[i]);

	return (rtn);
}

/**
 * arg_handler - Gets, calls, and runs the execution of a command.
 * @exe_ret: The return value of the parent process' last executed command.
 *
 * Return: The exit value of the last executed command.
 */

int arg_handler(int *exe_ret)
{
	char **args, *line = NULL, **front;
	int rtn = 0, index;

	line = get_cmd_in(line, exe_ret);
	if (!line)
		return (END_OF_FILE);

	args = _strtok(line, " ");
	free(line);
	if (!args)
		return (rtn);
	if (arg_checker(args) != 0)
	{
		*exe_ret = 2;
		free_args(args, args);
		return (*exe_ret);
	}
	front = args;

	for (index = 0; args[index]; index++)
	{
		if (_strncmp(args[index], ";", 1) == 0)
		{
			free(args[index]);
			args[index] = NULL;
			rtn = cmd_call(args, front, exe_ret);
			args = &args[++index];
			index = 0;
		}
	}
	if (args)
		rtn = cmd_call(args, front, exe_ret);

	free(front);
	return (rtn);
}

/**
 * arg_checker - Checks if there are any leading ';', ';;', '&&', or '||'.
 * @args: 2D pointer to tokenized commands and arguments.
 *
 * Return: - 2 - If the chars placed at an invalid position
 * O/w - 0.
 */

int arg_checker(char **args)
{
	size_t i = 0;
	char *cur, *nxt;

	for (; args[i]; i++)
	{
		cur = args[i];
		if (cur[0] == ';' || cur[0] == '&' || cur[0] == '|')
		{
			if (i == 0 || cur[1] == ';')
				return (custom_err(&args[i], 2));
			nxt = args[i + 1];
			if (nxt && (nxt[0] == ';' || nxt[0] == '&' || nxt[0] == '|'))
				return (custom_err(&args[i + 1], 2));
		}
	}
	return (0);
}
