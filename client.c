#include "minitalk.h"

volatile int ack_received = 0;

void handle_ack(int sig)
{
	if (sig == SIGUSR1)
		ack_received = 1;
}

void error_handler(char *message)
{
    printf("Error: %s\n", message);
    exit(EXIT_FAILURE);
}

void	send_message(pid_t pid, char c)
{
	int	i;

	i = 0;
	while (i < 8)
	{
		if ((c >> i) & 1)
		{
			if (kill(pid, SIGUSR2) == -1)
			{
				error_handler("Failed to send SIGUSR2");
			}
		}
		else
		{
			if (kill(pid, SIGUSR1) == -1)
			{
				error_handler("Failed to send SIGUSR2");
			}
		}
		i++;
		usleep(1000);
	}
	while (!ack_received)
	{
		pause();
	}
	ack_received = 0;
}

int	main(int argc, char *argv[])
{
	if (argc != 3)
	{
		printf("<PID> <message>");
	}
	struct sigaction sa;
	sa.sa_handler = handle_ack;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	if(sigaction(SIGUSR1,&sa,NULL)==-1)
	{
	    perror("sigaction");
	    exit(EXIT_FAILURE);
	}
	pid_t pid = atoi(argv[1]);
	char *message = argv[2];
	int i;
	i = 0;
	while (message[i])
	{
		send_message(pid, message[i]);
		i++;
	}
	return (0);
}