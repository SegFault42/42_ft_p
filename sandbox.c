#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <sys/wait.h>
#include <fcntl.h>

int	main()
{
	int	child_pid;
	char	*const arg[] = {"ls", "-laR", NULL};

	int fd = open("./file", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

	dup2(fd, 1);
	dup2(fd, 2);

	child_pid = fork();
	if (child_pid == -1)
	{
		perror("can't fork : ");
		exit(errno);
	}
	else if (child_pid == 0)
	{
		execv("/bin/ls", arg);
		exit(0);
	}
	else
	{
		wait(0);
		write(1, "finish\n", 7);
	}
}
