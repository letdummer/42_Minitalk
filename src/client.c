/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldummer- <ldummer-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 12:37:54 by ldummer-          #+#    #+#             */
/*   Updated: 2025/07/01 13:33:11 by ldummer-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void	message_received(int signum)
{
	if (signum == SIGUSR1)
	{
		ft_printf("Message sent successfully!\n");
		exit(0);
	}
}

void	send_char(int pid, char c)
{
	int	bit;

	bit = 0;
	while (bit < 8)
	{
		if ((c >> bit) & 1)
			kill(pid, SIGUSR1);
		else
			kill(pid, SIGUSR2);
		usleep(300);
		bit++;
	}
}

void	send_length(int pid, size_t length)
{
	size_t	bit;

	bit = 0;
	while (bit < (sizeof(size_t) * 8))
	{
		if ((length >> bit) & 1)
			kill(pid, SIGUSR1);
		else
			kill(pid, SIGUSR2);
		usleep(100);
		bit++;
	}
}

int	main(int ac, char **av)
{
	int					pid;
	int					i;
	char				*message;
	size_t				message_length;
	struct sigaction	sa;

	if (ac != 3)
		exit(ft_printf("Usage: ./client [PID] [MESSAGE]\n"));
	sa.sa_handler = message_received;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGUSR1, &sa, NULL);
	pid = ft_atoi(av[1]);
	if (pid <= 0)
		exit(ft_printf("Invalid PID\n"));
	if (kill(pid, 0) < 0)
		exit(ft_printf("Process does not exist\n"));
	message = av[2];
	message_length = ft_strlen(message);
	send_length(pid, message_length);
	i = 0;
	while (message[i])
		send_char(pid, message[i++]);
	send_char(pid, '\0');
	pause();
}
