#ifndef _SHELL_H_
#define _SHELL_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>

/* Buffer sizes for read and write operations */
#define READ_BUF_SIZE 1024
#define WRITE_BUF_SIZE 1024
#define BUF_FLUSH -1

/* Constants for command chaining */
#define CMD_NORM	0   /* Normal command */
#define CMD_OR		1   /* Command with logical OR (||) chaining */
#define CMD_AND		2   /* Command with logical AND (&&) chaining */
#define CMD_CHAIN	3   /* Command with sequential (;) chaining */

/* Flags for convert_number() */
#define CONVERT_LOWERCASE	1   /* Convert numbers to lowercase */
#define CONVERT_UNSIGNED	2   /* Convert numbers to unsigned format */

/* Set to 1 if using system getline() */
#define USE_GETLINE 0
#define USE_STRTOK 0

#define HIST_FILE	".simple_shell_history"   /* History file name */
#define HIST_MAX	4096                    /* Maximum history size */

extern char **environ;   /* External environment variable list */

/**
 *  * struct liststr - singly linked list
 *   * @num: the number field
 *    * @str: a string
 *     * @next: points to the next node
 */
typedef struct liststr
{
		int num;
			char *str;
				struct liststr *next;
} list_t;

/**
 *  * struct passinfo - contains pseudo-arguments to pass into a function,
 *   * allowing a uniform prototype for a function pointer struct
 *    * @arg: a string generated from getline containing arguments
 *     * @argv: an array of strings generated from arg
 *      * @path: a string path for the current command
 *       * @argc: the argument count
 *        * @line_count: the error count
 *         * @err_num: the error code for exit() calls
 *          * @linecount_flag: if on, count this line of input
 *           * @fname: the program filename
 *            * @env: linked list local copy of environ
 *             * @environ: custom modified copy of environ from LL env
 *              * @history: the history node
 *               * @alias: the alias node
 *                * @env_changed: on if environ was changed
 *                 * @status: the return status of the last exec'd command
 *                  * @cmd_buf: address of pointer to cmd_buf, on if chaining
 *                   * @cmd_buf_type: CMD_type (||, &&, ;) for command chaining
 *                    * @readfd: the file descriptor which to read line input
 *                     * @histcount: the history line number count
 */
typedef struct passinfo
{
		char *arg;
			char **argv;
				char *path;
					int argc;
						unsigned int line_count;
							int err_num;
								int linecount_flag;
									char *fname;
										list_t *env;
											list_t *history;
												list_t *alias;
													char **environ;
														int env_changed;
															int status;

										char **cmd_buf;     /* Pointer to cmd ; chain buffer */
																	int cmd_buf_type;   /* CMD_type (||, &&, ;) */
																		int readfd;
																			int histcount;
} info_t;

#define INFO_INIT \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
			0, 0, 0}

/**
 *  * struct builtin - contains a builtin string and related function
 *   * @type: the builtin command flag
 *    * @func: the function pointer to the builtin function
 */
typedef struct builtin
{
		char *type;
			int (*func)(info_t *);
} builtin_table;


/* Function prototypes */

/* toem_shloop.c */
int shell_loop(info_t *, char **);
int find_builtin(info_t *);
void find_cmd(info_t *);
void fork_cmd(info_t *);

/* toem_parser.c */
int is_cmd(info_t *, char *);
char *duplicate_chars(char *, int, int);
char *find_path(info_t *, char *, char *);

/* loophsh.c */
int loophsh(char **);

/* toem_errors.c */
void print_error(char *);
int print_char_error(char);
int print_fd(char c, int fd);
int print_str_fd(char *str, int fd);

/* toem_string.c */
int string_length(char *);
int string_compare(char *, char *);
char *starts_with(const char *, const char *);
char *string_concatenate(char *, char *);

/* toem_string1.c */
char *string_copy(char *, char *);
char *string_duplicate(const char *);
void print_string(char *);
int print_character(char);

/* toem_exits.c */
char *string_copy_n(char *, char *, int);
char *string_concatenate_n(char *, char *, int);
char *string_character(char *, char);

/* toem_tokenizer.c */
char **string_to_words(char *, char *);
char **string_to_words2(char *, char);

/* toem_realloc.c */
char *memory_set(char *, char, unsigned int);
void free_array(char **);
void *custom_realloc(void *, unsigned int, unsigned int);

/* toem_memory.c */
int buffer_free(void **);

/* toem_atoi.c */
int interactive_mode(info_t *);
int is_delimiter(char, char *);
int is_alphabetic(int);
int string_to_integer(char *);

/* toem_errors1.c */
int error_string_to_integer(char *);
void print_error_message(info_t *, char *);
int print_integer(int, int);
char *convert_number_to_string(long int, int, int);
void remove_comments_from_string(char *);

/* toem_builtin.c */
int builtin_exit(info_t *);
int builtin_cd(info_t *);
int builtin_help(info_t *);

/* toem_builtin1.c */
int builtin_history(info_t *);
int builtin_alias(info_t *);

/* toem_getline.c */
ssize_t get_input(info_t *);
int custom_getline(info_t *, char **, size_t *);
void signal_handler(int);

/* toem_getinfo.c */
void clear_info(info_t *);
void set_info(info_t *, char **);
void free_info(info_t *, int);

/* toem_environ.c */
char *get_environment_variable(info_t *, const char *);
int builtin_env(info_t *);
int builtin_setenv(info_t *);
int builtin_unsetenv(info_t *);
int populate_env_list(info_t *);

/* toem_getenv.c */
char **get_environ_list(info_t *);
int environment_unset(info_t *, char *);
int environment_set(info_t *, char *, char *);

/* toem_history.c */
char *get_history_file_path(info_t *info);
int write_history_to_file(info_t *info);
int read_history_from_file(info_t *info);
int build_history_list(info_t *info, char *buf, int linecount);
int renumber_history_lines(info_t *info);

/* toem_lists.c */
list_t *add_node(list_t **, const char *, int);
list_t *add_node_end(list_t **, const char *, int);
size_t print_string_list(const list_t *);
int delete_node_at_index(list_t **, unsigned int);
void free_list_memory(list_t **);

/* toem_lists1.c */
size_t list_length(const list_t *);
char **list_to_string_array(list_t *);
size_t print_list(const list_t *);
list_t *find_node_starts_with(list_t *, char *, char);
ssize_t get_node_index(list_t *, list_t *);

/* toem_vars.c */
int is_command_chained(info_t *, char *, size_t *);
void check_command_chaining(info_t *, char *, size_t *, size_t, size_t);
int replace_alias_with_value(info_t *);
int replace_variables(info_t *);
int replace_string(char **, char *);

#endif

