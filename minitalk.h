#ifndef MINITALK_H
#define MINITALK_H

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void handle_ack(int sig);
void error_handler(char *message);
void	send_message(pid_t pid, char c);
void	signal_handler(int sig, siginfo_t *info, void *context);

#endif