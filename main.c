#include "shell.h"

void signalHandler(int sig);
int execute(char **args, char **front);

/**
 * signalHandler - Avoids current process to finish and prints a new prompt.
 * @sig: The signal recieved.
 *
 */

void signalHandler(int sig)
{
	char *new_prompt = "\n#cisfun$ ";

	(void)sig;
	signal(SIGINT, signalHandler);
	write(STDIN_FILENO, new_prompt, 10);
}

/**
 * execute - Executes a command in the child process.
 * @args: An array of arguments.
 * @front: A double pointer that points to the first args.
 *
 * Return: The exit value of the last executed command.
 * or a corresponding error code, on error.
 *
 */

int execute(char **args, char **front)
{
	pid_t child_pid;
	char *cmd = args[0];
	int status, flag = 0;
	int ret = 0;

	if (cmd[0] != '/' && cmd[0] != '.')
	{
		flag = 1;
		cmd = get_cmd(cmd);
	}
	if (!cmd || (access(cmd, F_OK) == -1))
	{
		if (errno == EACCES)
			ret = (custom_err(args, 126));
		else
			ret = (custom_err(args, 127));
	}
	else
	{
		child_pid = fork();
		if (child_pid == -1)
		{
			if (flag)
				free(cmd);
			perror("Error child:");
			return (1);
		}
		if (child_pid == 0)
		{
			execve(cmd, args, environ);
			if (errno == EACCES)
				ret = (custom_err(args, 126));
			free_env();
			free_args(args, front);
			free_alias_list(aliases);
			_exit(ret);
		}
		else
		{
			wait(&status);
			ret = WEXITSTATUS(status);
		}
	}
	if (flag)
		free(cmd);
	return (ret);
}

/**
 * main - Entry point
 * @argc: Argument Counter
 * @argv: Argument Vector
 *
 * Return: 0 on success
 */

int main(int argc, char **argv)
{
	int ret = 0, rtn;
	int *exec_rtn = &rtn;
	char *prompt = "#cisfun$ ", *new_line = "\n";

	name = argv[0];
	hist = 1;
	aliases = NULL;
	signal(SIGINT, signalHandler);

	*exec_rtn = 0;
	environ = cpyenv();
	if (environ == NULL)
		exit(-100);

	if (argc != 1)
	{
		ret = proc_file_commands(argv[1], exec_rtn);
		free_env();
		free_alias_list(aliases);
		return (*exec_rtn);
	}

	if (isatty(STDIN_FILENO) == 0)
	{
		while (ret != END_OF_FILE && ret != EXIT)
			ret = arg_handler(exec_rtn);
		free_env();
		free_alias_list(aliases);

		return (*exec_rtn);
	}

	while (1)
	{
		write(STDOUT_FILENO, prompt, 9);
		ret = arg_handler(exec_rtn);
		if (ret == END_OF_FILE || ret == EXIT)
		{
			if (ret == END_OF_FILE)
				write(STDOUT_FILENO, new_line, 1);
			free_env();
			free_alias_list(aliases);
			exit(*exec_rtn);
		}
	}

	free_env();
	free_alias_list(aliases);
	return (*exec_rtn);
}
