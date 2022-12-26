/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwong <hwong@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/21 09:23:22 by hwong             #+#    #+#             */
/*   Updated: 2022/12/21 09:23:22 by hwong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <unistd.h>

/*
	bitwise operators:

	AND &  -  1 if only both are 1
	1001 0110 & 1000 0010 = 1000 0010
	
	OR |   -  1 if either is 1
	1001 0110 & 1000 0011 = 1001 0111
*/

void	putnum(int i)
{
	char	c;

	if (i > 9)
		putnum(i / 10);
	c = i % 10 + 48;
	write(1, &c, 1);
}

void	bit_handler(int sig, siginfo_t *info, void *data)
{
	static int	i;
	// c is 0000 0000 by default once initialised
	static char	c;

	(void) data;
	if (sig == SIGUSR1)
	// bitwise OR to write 0 to 1
		c |= (1 << i);
	// static i increment so we know when a full character is recieved
	i++;
	// if i is at end of byte
	if (i == 8)
	{
		write(1, &c, 1);
		// if the char is the null byte
		if (c == '\0')
		{
			// send signal to client PID, SIGUSR1
			kill(info->si_pid, SIGUSR1);
			write(1, "\n", 1);
		}
		// reset static vars
		i = 0;
		c = 0;
	}
}

int	main(void)
{
	// server process ID
	int					pid;
	// signal handler
	struct sigaction	sa;

	// getpid() function will return an integer process ID value
	pid = getpid();
	write(1, "Server PID: ", 12);
	putnum (pid);
	write(1, "\n", 1);
	// handle recieved signal using specified function
	sa.sa_sigaction = bit_handler;
	// contains client PID
	sa.sa_flags = SA_SIGINFO;
	// if statement is 1, then always True, indefinite loop
	while (1)
	{
		// wait for SIGUSR1 signal
		sigaction(SIGUSR1, &sa, NULL);
		// wait for SIGUSR2 signal
		sigaction(SIGUSR2, &sa, NULL);
		pause();
	}
	return (0);
}
