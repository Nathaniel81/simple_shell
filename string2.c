#include "shell.h"

int _strcmp(char *s1, char *s2);
int _strncmp(const char *s1, const char *s2, size_t n);
int _strspn(char *s, char *accept);
char *_strchr(char *s, char c);

/**
 * _strchr - Locates a character in a string.
 * @s: The string input
 * @c: The character input
 *
 * Return: A pointer to the first match.
 *         o/w NULL.
 *
 */

char *_strchr(char *s, char c)
{
	int index = 0;

	for (; s[index]; index++)
	{
		if (s[index] == c)
			return (s + index);
	}

	return (NULL);
}

/**
 * _strspn - Gets the length of a prefix substring.
 * @s: The string passed
 * @accept: The prefix to be measured.
 *
 * Return: The number of bytes
 */

int _strspn(char *s, char *accept)
{
	int bytes = 0;
	int index;

	while (*s)
	{
		for (index = 0; accept[index]; index++)
		{
			if (*s == accept[index])
			{
				bytes++;
				break;
			}
		}
		s++;
	}
	return (bytes);
}

/**
 * _strcmp - Compares two strings.
 * @s1: String input 1.
 * @s2: String input 2.
 *
 * Return: 0 if s1 = s2
 */

int _strcmp(char *s1, char *s2)
{
	while (*s1 && *s2 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	if (*s1 != *s2)
		return (*s1 - *s2);

	return (0);
}

/**
 * _strncmp - Compare two strings.
 * @s1: Pointer to the first string.
 * @s2: Pointer to the second string.
 * @n: The first n bytes of the strings to compare.
 *
 * Return: 0 if s1 and s2 match.
 */

int _strncmp(const char *s1, const char *s2, size_t n)
{
	size_t i;

	for (i = 0; s1[i] && s2[i] && i < n; i++)
	{
		if (s1[i] > s2[i])
			return (s1[i] - s2[i]);
		else if (s1[i] < s2[i])
			return (s1[i] - s2[i]);
	}
	if (i == n)
		return (0);
	else
		return (-15);
}
