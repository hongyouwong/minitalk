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
	static char	c;

	(void) data;
	if (sig == SIGUSR1)
		c |= (1 << i);
	i++;
	if (i == 8)
	{
		write(1, &c, 1);
		if (c == '\0')
		{
			kill(info->si_pid, SIGUSR1);
			write(1, "\n", 1);
		}
		i = 0;
		c = 0;
	}
}

int	main(void)
{
	int					pid;
	struct sigaction	sa;

	pid = getpid();
	write(1, "Server PID: ", 12);
	putnum (pid);
	write(1, "\n", 1);
	sa.sa_sigaction = bit_handler;
	sa.sa_flags = SA_SIGINFO;
	while (1)
	{
		sigaction(SIGUSR1, &sa, NULL);
		sigaction(SIGUSR2, &sa, NULL);
		pause();
	}
	return (0);
}
