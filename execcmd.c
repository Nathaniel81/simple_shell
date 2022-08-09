#include "shell.h"

int fail_to_open(char *file_path);
int proc_file_commands(char *file_path, int *exec_rtn);

/**
 * fail_to_open - print an err, if the file doesn't exist.
 * @file_path: Path to the file.
 *
 * Return: an int.
 */

int fail_to_open(char *file_path)
{
	int length;
	char *err, *hist_str;

	hist_str = _itoa(hist);
	if (!hist_str)
		return (127);

	length = _strlen(name) + _strlen(hist_str) + _strlen(file_path) + 16;
	err = malloc(sizeof(char) * (length + 1));
	if (err == NULL)
	{
		free(hist_str);
		return (127);
	}

	_strcpy(err, name);
	_strcat(err, ": ");
	_strcat(err, hist_str);
	_strcat(err, ": Can't open ");
	_strcat(err, file_path);
	_strcat(err, "\n");

	free(hist_str);
	write(STDERR_FILENO, err, length);
	free(err);

	return (127);
}

/**
 * proc_file_commands - Takes a file and attempts to execute the
 * commands contained within it.
 * @file_path: Path to the file.
 * @exec_rtn: Returns the value returned by the last command executed.
 *
 * Return: The value of the last command ran, on success
 * on error 127 is returned
 *
 */

int proc_file_commands(char *file_path, int *exec_rtn)
{
	int rtn;
	ssize_t file, b_read, i;
	unsigned int old_size = 120;
	unsigned int line_size = 0;
	char *line, **args, **front;
	char buffer[120];

	hist = 0;
	file = open(file_path, O_RDONLY);
	if (file == -1)
	{
		*exec_rtn = fail_to_open(file_path);
		return (*exec_rtn);
	}
	line = malloc(sizeof(char) * old_size);
	if (line == NULL)
		return (-1);
	do {
		b_read = read(file, buffer, 119);
		if (b_read == 0 && line_size == 0)
			return (*exec_rtn);
		buffer[b_read] = '\0';
		line_size += b_read;
		line = _realloc(line, old_size, line_size);
		_strcat(line, buffer);
		old_size = line_size;
	} while (b_read);
	for (i = 0; line[i] == '\n'; i++)
		line[i] = ' ';
	for (; i < line_size; i++)
	{
		if (line[i] == '\n')
		{
			line[i] = ';';
			for (i += 1; i < line_size && line[i] == '\n'; i++)
				line[i] = ' ';
		}
	}
	var_rep(&line, exec_rtn);
	handle_line(&line, line_size);
	args = _strtok(line, " ");
	free(line);
	if (!args)
		return (0);
	if (arg_checker(args) != 0)
	{
		*exec_rtn = 2;
		free_args(args, args);
		return (*exec_rtn);
	}
	front = args;

	for (i = 0; args[i]; i++)
	{
		if (_strncmp(args[i], ";", 1) == 0)
		{
			free(args[i]);
			args[i] = NULL;
			rtn = cmd_call(args, front, exec_rtn);
			args = &args[++i];
			i = 0;
		}
	}

	rtn = cmd_call(args, front, exec_rtn);

	free(front);

	return (rtn);
}
