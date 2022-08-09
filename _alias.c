#include "shell.h"

void set_alias(char *var_name, char *value);
int _alias(char **args, char __attribute__((__unused__)) **front);
void print_alias(alias_t *alias);

/**
 * _alias - Builtin commands for aliases
 * @args: An array of arguments.
 * @front: A double pointer to the beginning of args.
 *
 * Return: 0 on success.
 * o/w NULL is returned
 */

int _alias(char **args, char __attribute__((__unused__)) **front)
{
	alias_t *temp = aliases;
	char *value;
	int i = 0, rtn = 0;

	if (!args[0])
	{
		while (temp)
		{
			print_alias(temp);
			temp = temp->next;
		}
		return (rtn);
	}
	for (; args[i]; i++)
	{
		temp = aliases;
		value = _strchr(args[i], '=');
		if (!value)
		{
			while (temp)
			{
				if (_strcmp(args[i], temp->name) == 0)
				{
					print_alias(temp);
					break;
				}
				temp = temp->next;
			}
			if (!temp)
				rtn = custom_err(args + i, 1);
		}
		else
			set_alias(args[i], value);
	}
	return (rtn);
}

/**
 * set_alias - Will either replace an existing alias 'name' with a new value
 * or create a new one.
 * @var_name: Name of the alias.
 * @value: Value of the alias.
 *
 */

void set_alias(char *var_name, char *value)
{
	alias_t *temp = aliases;
	int length, j, k;
	char *new_value;

	*value = '\0';
	value++;
	length = _strlen(value) - _strspn(value, "'\"");
	new_value = malloc(sizeof(char) * (length + 1));
	if (!new_value)
		return;
	for (j = 0, k = 0; value[j]; j++)
	{
		if (value[j] != '\'' && value[j] != '"')
			new_value[k++] = value[j];
	}
	new_value[k] = '\0';
	while (temp)
	{
		if (_strcmp(var_name, temp->name) == 0)
		{
			free(temp->value);
			temp->value = new_value;
			break;
		}
		temp = temp->next;
	}
	if (!temp)
		add_alias_end(&aliases, var_name, new_value);
}

/**
 * print_alias - Prints the alias.
 * @alias: Pointer to an alias.
 *
 */

void print_alias(alias_t *alias)
{
	char *alias_string;
	int length = _strlen(alias->name) + _strlen(alias->value) + 4;

	alias_string = malloc(sizeof(char) * (length + 1));
	if (!alias_string)
		return;
	_strcpy(alias_string, alias->name);
	_strcat(alias_string, "='");
	_strcat(alias_string, alias->value);
	_strcat(alias_string, "'\n");

	write(STDOUT_FILENO, alias_string, length);
	free(alias_string);
}

/**
 * aliase_rep - Iterates over the parameters
 * and replaces any aliases that match.
 * @args: Pointer to the arguments.
 *
 * Return: Pointer to the arguments.
 */

char **aliase_rep(char **args)
{
	alias_t *temp;
	int i = 0;
	char *new_value;

	if (_strcmp(args[0], "alias") == 0)
		return (args);
	for (; args[i]; i++)
	{
		temp = aliases;
		while (temp)
		{
			if (_strcmp(args[i], temp->name) == 0)
			{
				new_value = malloc(sizeof(char) * (_strlen(temp->value) + 1));
				if (!new_value)
				{
					free_args(args, args);
					return (NULL);
				}
				_strcpy(new_value, temp->value);
				free(args[i]);
				args[i] = new_value;
				i--;
				break;
			}
			temp = temp->next;
		}
	}

	return (args);
}
