#ifndef SHELL_H
#define SHELL_H

#include <sys/wait.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
/* open */
#include <sys/types.h>
#include <signal.h>
#include <fcntl.h>

/* FLAGS */
#define END_OF_FILE -2
#define EXIT -3


extern char **environ;
char *name;
int hist;

/**
 * struct list_s - A new struct type that defines a linked list.
 * @dir: A directory path.
 * @next: A pointer to the next node.
 *
 */
typedef struct list_s
{
	char *dir;
	struct list_s *next;
} list_t;

/**
 * struct builtin_s - A new struct type that defines builtin commands.
 * @name: Name of the builtin command.
 * @f: A function pointer to the function of the built-in command.
 *
 */
typedef struct builtin_s
{
	char *name;
	int (*f)(char **argv, char **front);
} builtin_t;

/**
 * struct alias_s - A new struct defining aliases.
 * @name: Name of the alias.
 * @value: Value of the alias.
 * @next: A pointer to the next node.
 *
 */
typedef struct alias_s
{
	char *name;
	char *value;
	struct alias_s *next;
} alias_t;

alias_t *aliases;

/* Helpers */
char **_strtok(char *line, char *delim);
char **_getenv(const char *var);
ssize_t _getline(char **lineptr, size_t *n, FILE *stream);
list_t *get_path_dir(char *path);
int execute(char **args, char **front);
void *_realloc(void *ptr, unsigned int old_size, unsigned int new_size);
char *_itoa(int num);
void free_list(list_t *head);
char *get_cmd(char *command);
char *get_cmd_in(char *line, int *exe_ret);
int run_cmd(char **args, char **front, int *exe_ret);
void var_rep(char **args, int *exe_ret);
int arg_checker(char **args);
char **aliase_rep(char **args);
int cmd_call(char **args, char **front, int *exe_ret);
void handle_line(char **line, ssize_t read);
void free_args(char **args, char **front);
int arg_handler(int *exe_ret);
char **cpyenv(void);
void free_env(void);
void alias_helper(void);
void hist_helper(void);
void unsetenv_helper(void);
void exit_helper(void);
void cd_helper(void);
void _helper(void);
void env_helper(void);
void help_all(void);
void setenv_helper(void);
int proc_file_commands(char *file_path, int *exe_ret);

/* String functions */
int _strlen(const char *s);
char *_strcpy(char *dest, const char *src);
int _strcmp(char *s1, char *s2);
int _strncmp(const char *s1, const char *s2, size_t n);
int _strspn(char *s, char *accept);
char *_strchr(char *s, char c);
char *_strcat(char *dest, const char *src);
char *_strncat(char *dest, const char *src, size_t n);

/* Builtins */
int _b_exit(char **args, char **front);
int _unsetenv(char **args, char __attribute__((__unused__)) **front);
int _setenv(char **args, char __attribute__((__unused__)) **front);
int (*get_builtin(char *command))(char **args, char **front);
int _alias(char **args, char __attribute__((__unused__)) **front);
int _help(char **args, char __attribute__((__unused__)) **front);
int _cd(char **args, char __attribute__((__unused__)) **front);
int shellby_env(char **args, char __attribute__((__unused__)) **front);

/* Handlers */
char *err_1(char **args);
char *err_env(char **args);
char *err_126(char **args);
char *err_2_cd(char **args);
char *err_127(char **args);
char *err_2_exit(char **args);
char *err_2_syn(char **args);
int custom_err(char **args, int err);

/* Linkedlists */
void free_alias_list(alias_t *head);
alias_t *add_alias_end(alias_t **head, char *name, char *value);
void free_list(list_t *head);
list_t *add_node_end(list_t **head, char *dir);

#endif
