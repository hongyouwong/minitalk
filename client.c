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


/*	1 char == 1 byte
	1 byte == 8 bits

	every bit is either a 0 or a 1

	bitwise operators:
	byte >> x  -  shift bits to right by x
	1011 0110     0101 1011

	byte << y  -   shift bits to left by x

*/

void	send_bit(int pid, char *str, size_t len)
{
	int		shift;
	size_t	i;

	i = 0;
	// for each char in the str
	while (i < len)
	{
		shift = 0;
		// for each bit in the char
		// 01000011
		while (shift < 8)
		{
			// if current bit is 1, send SIGUSR1
			if ((str[i] >> shift) & 1)
				kill(pid, SIGUSR1);
			// else if bit is 0, send SIGUSR2
			else
				kill(pid, SIGUSR2);
			// move to next bit
			shift++;
			// delay to declutter server
			usleep(100);
		}
		// move to next char
		i++;
	}
	i = -1;
	// null byte
	while (++i < 8)
	{
		// 8 0s for a null
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
	// server PID
	int					pid;
	int					slen;

	slen = 0;
	if (argc == 3)
	{
		pid = ft_atoi(argv[1]);
		while (argv[2][slen])
			slen++;
		// wait for return echo signal from server
		if (signal(SIGUSR1, clientEcho))
			return (0);
		// handle sending
		clientSend(pid, argv[2], slen);
	}
	else
		write(1, "\nInvalid input.", 15);
	return (0);
}
