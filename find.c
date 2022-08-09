#include "shell.h"

char *fill_path_dir(char *path);
list_t *get_path_dir(char *path);

/**
 * get_cmd - Locates a command in the PATH.
 * @command: The command to locate.
 *
 * Return: the full pathname of the command.
 */

char *get_cmd(char *command)
{
	char **path, *temp;
	list_t *dirs, *head;
	struct stat st;

	path = _getenv("PATH");
	if (!path || !(*path))
		return (NULL);

	dirs = get_path_dir(*path + 5);
	head = dirs;

	while (dirs)
	{
		temp = malloc(_strlen(dirs->dir) + _strlen(command) + 2);
		if (!temp)
			return (NULL);

		_strcpy(temp, dirs->dir);
		_strcat(temp, "/");
		_strcat(temp, command);

		if (stat(temp, &st) == 0)
		{
			free_list(head);
			return (temp);
		}

		dirs = dirs->next;
		free(temp);
	}

	free_list(head);

	return (NULL);
}

/**
 * fill_path_dir - Copies path
 * but additionally substitutes leading/sandwiched/trailing colons (:)
 * with the current working directory.
 * @path: The colon-separated list of directories.
 *
 * Return: A copy of path with any leading/sandwiched/trailing colons replaced
 * with the current working directory.
 */

char *fill_path_dir(char *path)
{
	int i = 0, length = 0;
	char *cpyPath, *pwd;

	pwd = *(_getenv("PWD")) + 4;
	for (; path[i]; i++)
	{
		if (path[i] == ':')
		{
			if (path[i + 1] == ':' || i == 0 || path[i + 1] == '\0')
				length += _strlen(pwd) + 1;
			else
				length++;
		}
		else
			length++;
	}
	cpyPath = malloc(sizeof(char) * (length + 1));
	if (!cpyPath)
		return (NULL);
	cpyPath[0] = '\0';
	for (i = 0; path[i]; i++)
	{
		if (path[i] == ':')
		{
			if (i == 0)
			{
				_strcat(cpyPath, pwd);
				_strcat(cpyPath, ":");
			}
			else if (path[i + 1] == ':' || path[i + 1] == '\0')
			{
				_strcat(cpyPath, ":");
				_strcat(cpyPath, pwd);
			}
			else
				_strcat(cpyPath, ":");
		}
		else
		{
			_strncat(cpyPath, &path[i], 1);
		}
	}
	return (cpyPath);
}

/**
 * get_path_dir - Tokenizes a list of directories
 * separated by colons into a linked list.
 * @path: The colon-separated list of directories.
 *
 * Return: A pointer to the initialized linked list.
 */

list_t *get_path_dir(char *path)
{
	int index;
	char **dirs, *cpyPath;
	list_t *head = NULL;

	cpyPath = fill_path_dir(path);
	if (!cpyPath)
		return (NULL);
	dirs = _strtok(cpyPath, ":");
	free(cpyPath);
	if (!dirs)
		return (NULL);

	for (index = 0; dirs[index]; index++)
	{
		if (add_node_end(&head, dirs[index]) == NULL)
		{
			free_list(head);
			free(dirs);
			return (NULL);
		}
	}
	free(dirs);
	return (head);
}
