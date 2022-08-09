#include "shell.h"

void free_env(void);
char **cpyenv(void);
char **_getenv(const char *var);

/**
 * cpyenv - Creates a copy of the environment.
 *
 * Return: A double pointer to the new copy.
 * o/w - NULL.
 *
 */

char **cpyenv(void)
{
	char **new_environ;
	size_t size = 0;
	int index = 0;

	for (; environ[size]; size++)
		;

	new_environ = malloc(sizeof(char *) * (size + 1));
	if (!new_environ)
		return (NULL);

	for (; environ[index]; index++)
	{
		new_environ[index] = malloc(_strlen(environ[index]) + 1);

		if (!new_environ[index])
		{
			for (index--; index >= 0; index--)
				free(new_environ[index]);
			free(new_environ);
			return (NULL);
		}
		_strcpy(new_environ[index], environ[index]);
	}
	new_environ[index] = NULL;

	return (new_environ);
}

/**
 * free_env - Frees the the environment copy.
 *
 */

void free_env(void)
{
	int index = 0;

	for (; environ[index]; index++)
		free(environ[index]);
	free(environ);
}

/**
 * _getenv - Retrieves an environmental variable from the PATH.
 * @var: The name of the environmental variable to be obtained.
 *
 * Return: A pointer to the environmental variable.
 */

char **_getenv(const char *var)
{
	int index, length;

	length = _strlen(var);
	for (index = 0; environ[index]; index++)
	{
		if (_strncmp(var, environ[index], length) == 0)
			return (&environ[index]);
	}

	return (NULL);
}
