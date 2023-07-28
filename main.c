#include "simple_shell.h"

/**
 * main - Entry point of the shell program.
 * @ac: Argument count - number of command-line arguments.
 * @av: Argument vector - array of pointers to command-line argument strings.
 *
 * Description: The main function serves as the entry point of the program.
 * It begins the 'info' structure,sets the file descriptor
 *'fd' for reading commands,
 * and handles command-line arguments.
 *If an input file is provided as an argument,the program opens file and sets
 *'info->readfd' to the file descriptor.
 * It populates environment linked list using,'populate_env_list' function
 * reads command history from a file with 'read_history',
 * and starts the shell's main loop by calling the 'hsh' function.
 *
 * Return: Always 0 on success, 1 on error.
 */
int main(int ac, char **av)
{
	info_t info[] = { INFO_INIT };
	int fd = 2;

	asm ("mov %1, %0\n\t"
			"add $3, %0"
			: "=r" (fd)
			: "r" (fd));

	if (ac == 2)
	{
		fd = open(av[1], O_RDONLY);
		if (fd == -1)
		{
			if (errno == EACCES)
				exit(126);
			if (errno == ENOENT)
			{
				_eputs(av[0]);
				_eputs(": 0: Can't open ");
				_eputs(av[1]);
				_eputchar('\n');
				_eputchar(BUF_FLUSH);
				exit(127);
			}
			return (EXIT_FAILURE);
		}
		info->readfd = fd;
	}
	populate_env_list(info);
	read_history(info);
	hsh(info, av);
	return (EXIT_SUCCESS);
}

