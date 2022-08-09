#include "shell.h"
int _b_exit(char **args, char **front);
int _cd(char **args, char __attribute__((__unused__)) **front);
int (*get_builtin(char *command))(char **args, char **front);
int _help(char **args, char __attribute__((__unused__)) **front);

/**
 * get_builtin - Matches a command to a built-in function.
 * @command: The command to match.
 *
 * Return: A function pointer to the corresponding function.
 */

int (*get_builtin(char *command))(char **args, char **front)
{
	builtin_t funcs[] = {
		{ "exit", _b_exit },
		{ "env", shellby_env },
		{ "setenv", _setenv },
		{ "unsetenv", _unsetenv },
		{ "cd", _cd },
		{ "alias", _alias },
		{ "help", _help },
		{ NULL, NULL }
	};
	int i;

	for (i = 0; funcs[i].name; i++)
	{
		if (_strcmp(funcs[i].name, command) == 0)
			break;
	}
	return (funcs[i].f);
}

/**
 * _b_exit - Causes the shell's standard process termination.
 * @args: An array of arguments.
 * @front: Pointer to the beginning of args.
 *
 * Return: -3 - if there are no arguments.
 *         -2 - If the given exit value is invalid.
 *
 */

int _b_exit(char **args, char **front)
{
	int i, int_len = 10;
	unsigned int num = 0, max = 1 << (sizeof(int) * 8 - 1);

	if (args[0])
	{
		if (args[0][0] == '+')
		{
			i = 1;
			int_len++;
		}
		for (; args[0][i]; i++)
		{
			if (i <= int_len && args[0][i] >= '0' && args[0][i] <= '9')
				num = (num * 10) + (args[0][i] - '0');
			else
				return (custom_err(--args, 2));
		}
	}
	else
	{
		return (-3);
	}
	if (num > max - 1)
		return (custom_err(--args, 2));
	args -= 1;
	free_args(args, front);
	free_env();
	free_alias_list(aliases);
	exit(num);
}

/**
 * _cd - Changes the current directory of the process.
 * @args: An array of arguments.
 * @front: Pointer to the beginning of args.
 *
 * Return: 0 on success.
 *
 */

int _cd(char **args, char __attribute__((__unused__)) **front)
{
	char **dirInfo, *new_line = "\n";
	char *oldpwd = NULL, *pwd = NULL;
	struct stat dir;

	oldpwd = getcwd(oldpwd, 0);
	if (!oldpwd)
		return (-1);

	if (args[0])
	{
		if (*(args[0]) == '-' || _strcmp(args[0], "--") == 0)
		{
			if ((args[0][1] == '-' && args[0][2] == '\0') ||
					args[0][1] == '\0')
			{
				if (_getenv("OLDPWD") != NULL)
					(chdir(*_getenv("OLDPWD") + 7));
			}
			else
			{
				free(oldpwd);
				return (custom_err(args, 2));
			}
		}
		else
		{
			if (stat(args[0], &dir) == 0 && S_ISDIR(dir.st_mode)
					&& ((dir.st_mode & S_IXUSR) != 0))
				chdir(args[0]);
			else
			{
				free(oldpwd);
				return (custom_err(args, 2));
			}
		}
	}
	else
	{
		if (_getenv("HOME") != NULL)
			chdir(*(_getenv("HOME")) + 5);
	}

	pwd = getcwd(pwd, 0);
	if (!pwd)
		return (-1);

	dirInfo = malloc(sizeof(char *) * 2);
	if (!dirInfo)
		return (-1);

	dirInfo[0] = "OLDPWD";
	dirInfo[1] = oldpwd;
	if (_setenv(dirInfo, dirInfo) == -1)
		return (-1);

	dirInfo[0] = "PWD";
	dirInfo[1] = pwd;
	if (_setenv(dirInfo, dirInfo) == -1)
		return (-1);
	if (args[0] && args[0][0] == '-' && args[0][1] != '-')
	{
		write(STDOUT_FILENO, pwd, _strlen(pwd));
		write(STDOUT_FILENO, new_line, 1);
	}
	free(oldpwd);
	free(pwd);
	free(dirInfo);
	return (0);
}

/**
 * _help - Displays information about the builtin commands.
 * @args: An array of arguments.
 * @front: Pointer to the beginning of args.
 *
 * Return: -1 - If an error occurs
 *
 */

int _help(char **args, char __attribute__((__unused__)) **front)
{
	if (!args[0])
		help_all();
	else if (_strcmp(args[0], "alias") == 0)
		alias_helper();
	else if (_strcmp(args[0], "cd") == 0)
		cd_helper();
	else if (_strcmp(args[0], "exit") == 0)
		exit_helper();
	else if (_strcmp(args[0], "env") == 0)
		env_helper();
	else if (_strcmp(args[0], "setenv") == 0)
		setenv_helper();
	else if (_strcmp(args[0], "unsetenv") == 0)
		unsetenv_helper();
	else if (_strcmp(args[0], "help") == 0)
		_helper();
	else
		write(STDERR_FILENO, name, _strlen(name));

	return (0);
}
