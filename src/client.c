/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldummer- <ldummer-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 12:37:54 by ldummer-          #+#    #+#             */
/*   Updated: 2025/07/03 17:17:42 by ldummer-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

static int g_can_send_message = 0;

void	handle_server_response(int signum)
{
	if (signum == SIGUSR1 && g_can_send_message == 0)
	{
		g_can_send_message = 1;
		ft_printf("Can send message.\n");
	}
	else if (signum == SIGUSR1 && g_can_send_message == 1)
	{
		ft_printf("Full message received by server!\n");
		exit(0);
	}
	else if (signum == SIGUSR2 && g_can_send_message == 0)
	{
		ft_printf("Server couldn't allocate memory for message!\n");
		exit(1);
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
		usleep(200);
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

void	send_message(int pid, char *message)
{
	int	i;

	i = 0;
	if (g_can_send_message)
	{
		i = 0;
		while (message[i])
			send_char(pid, message[i++]);
		send_char(pid, '\0');
		pause();
	}
}

int	main(int ac, char **av)
{
	int					pid;
	char				*message;
	size_t				message_length;
	struct sigaction	sa;

	if (ac != 3)
		exit(ft_printf("Usage: ./client [PID] [MESSAGE]\n"));
	sa.sa_handler = handle_server_response;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGUSR1, &sa, NULL);
	sigaction(SIGUSR2, &sa, NULL);
	pid = ft_atoi(av[1]);
	if (pid <= 0)
		exit(ft_printf("Invalid PID.\n"));
	if (kill(pid, 0) < 0)
		exit(ft_printf("Process does not exist.\n"));
	message = av[2];
	message_length = ft_strlen(message);
	send_length(pid, message_length);
	pause();
	usleep(50);
	send_message(pid, message);
	return (0);
}
