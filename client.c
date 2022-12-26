/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwong <hwong@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/21 09:23:19 by hwong             #+#    #+#             */
/*   Updated: 2022/12/21 09:23:19 by hwong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <unistd.h>

void	send_bit(int pid, char *str, size_t len)
{
	int		shift;
	size_t	i;

	i = 0;
	while (i < len)
	{
		shift = 0;
		while (shift < 8)
		{
			if ((str[i] >> shift) & 1)
				kill(pid, SIGUSR1);
			else
				kill(pid, SIGUSR2);
			shift++;
			usleep(75);
		}
		i++;
	}
	i = -1;
	while (++i < 8)
	{
		kill(pid, SIGUSR2);
		usleep(20);
	}
}

int	ft_atoi(char *str)
{
	int	result;
	int	sign;

	result = 0;
	sign = 1;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	while (*str >= '0' && *str <= '9')
		result = result * 10 + *str++ - '0';
	return (sign * result);
}

void	clientEcho(int sig)
{
	if (sig == SIGUSR1)
		write(1, "\nTransmitted!\n", 14);
}

void	clientSend(int pid, char *str, int slen)
{
	signal(SIGUSR1, clientEcho);
	send_bit(pid, str, slen);
}

int	main(int argc, char **argv)
{
	int					pid;
	int					slen;

	slen = 0;
	if (argc == 3)
	{
		pid = ft_atoi(argv[1]);
		while (argv[2][slen])
			slen++;
		if (signal(SIGUSR1, clientEcho))
			return (0);
		clientSend(pid, argv[2], slen);
	}
	else
		write(1, "\nInvalid input.", 15);
	return (0);
}
