#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"

#define MAX_COMMAND_LENGTH 1024

/**
 * main - Entry point for the shell program.
 *
 * This function initializes the shell and starts the main loop for reading
 * user input, executing commands, and displaying the shell prompt.
 *
 * Return: Always returns 0.
 */
int main(void)
{
    char *line;
    char **args;
    int status;

    /* Main shell loop */
    while (1)
    {
        /* Print the shell prompt */
        printf("myshell> ");

        /* Read the command line input from the user */
        line = NULL;
        size_t buffer_size = 0;
        getline(&line, &buffer_size, stdin);

        /* Remove the newline character from the end of the input */
        line[strcspn(line, "\n")] = '\0';

        /* Split the command line input into individual arguments */
        args = splitstring(line, " "); /* You need to implement this function in shell_utils.c */

        /* Execute the command */
        execute(args); /* You need to implement this function in shell_exec.c */

        /* Free the dynamically allocated memory for the command and arguments */
        free(line);
        freearv(args);

        /* Optionally, add a way to handle built-in commands like 'exit', 'cd', etc. */
        /* For example, you can check if the first argument (args[0]) matches a built-in command. */

        /* Optionally, add a way to handle I/O redirection, background processes, etc. */
        /* For a more complete shell, you'll need to implement more features. */

        /* Wait for the child process to finish (if any) */
        wait(&status);
    }

    return (EXIT_SUCCESS);
}

