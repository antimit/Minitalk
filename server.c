#include "minitalk.h"



void error_handler(char *message)
{
    printf("Error: %s\n", message);
    exit(EXIT_FAILURE);
}
void	signal_handler(int sig, siginfo_t *info, void *context)
{
	static char	c = 0;
	static int	index = 0;
	pid_t		client_pid;

	if (info)
		client_pid = info->si_pid;
	if (sig == SIGUSR1)
		c |= (0 << index);
	else if (sig == SIGUSR2)
		c |= (1 << index);
	index++;
	if (index == 8)
	{
		write(1, &c, 1);
		index = 0;
		c = 0;
		if(client_pid > 0)
		{
			if(kill(client_pid,SIGUSR1)==-1)
				error_handler("Failed to send acknowledgment");
		}
	}

}

int	main(void)
{
	struct sigaction	sa;

	sa.sa_sigaction = &signal_handler;
	sa.sa_flags = SA_SIGINFO;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGUSR1, &sa, NULL) == -1 || sigaction(SIGUSR2, &sa, NULL) ==
		-1)
	{
		error_handler("sigaction");
		exit(EXIT_FAILURE);
	}
	printf("Server PID: %d\n", getpid());
	while (1)
		pause();
	return (0);
}
