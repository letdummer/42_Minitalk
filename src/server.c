/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldummer- <ldummer-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 12:41:52 by ldummer-          #+#    #+#             */
/*   Updated: 2025/07/03 17:22:58 by ldummer-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

static void	reset_server_state(t_data *data)
{
	if (data->message)
	{
		free(data->message);
		data->message = NULL;
	}
	data->bit_count = 0;
	data->c = 0;
	data->index = 0;
	data->message_length = 0;
	data->receiving_length = 1;
	data->length_bits_received = 0;
}

static int	handle_length(int signum, siginfo_t *info, t_data *data)
{
	if (signum == SIGUSR1)
		data->message_length |= (1UL << data->length_bits_received);
	data->length_bits_received++;
	if (data->length_bits_received == sizeof(size_t) * 8)
	{
		data->receiving_length = 0;
		data->message = malloc(data->message_length + 1);
		if (!data->message)
		{
			ft_printf("Memory allocation failed = %d\n",
			data->message_length + 1);
			reset_server_state(data);
			usleep(200);
			ft_printf("|Server reset and ready for new messages|\n");
			kill(info->si_pid, SIGUSR2);
			return (1);
		}
		ft_printf("Message length => %d\n", data->message_length);
		data->message[data->message_length] = '\0';
		usleep(200);
		kill(info->si_pid, SIGUSR1);
		ft_printf("\n");
	}
	return (0);
}

static void	handle_message(int signum, siginfo_t *info, t_data *data)
{
	if (signum == SIGUSR1)
		data->c = data->c | (1 << data->bit_count);
	data->bit_count++;
	if (data->bit_count == 8)
	{
		data->message[data->index++] = data->c;
		if (data->c == '\0')
		{
			ft_putstr_fd(data->message, 1);
			ft_putchar_fd('\n', 1);
			usleep(200);
			kill(info->si_pid, SIGUSR1);
			reset_server_state(data);
			usleep(200);
			ft_printf("|Server ready for new messages|\n");
		}
		data->bit_count = 0;
		data->c = 0;
	}
}

void	handler(int signum, siginfo_t *info, void *context)
{
	static t_data	data = {0, 0, NULL, 0, 0, 1, 0};

	(void)context;
	if (data.receiving_length)
	{
		if (handle_length(signum, info, &data))
			return ;
		return ;
	}
	handle_message(signum, info, &data);
}

int	main(int ac, char **av)
{
	int					pid;
	struct sigaction	sa;

	(void)av;
	if (ac != 1)
		exit(ft_printf("Usage: ./server\n"));
	pid = getpid();
	ft_printf("pid: %d\n", pid);
	sa.sa_sigaction = handler;
	sa.sa_flags = SA_SIGINFO;
	sigaction(SIGUSR1, &sa, NULL);
	sigaction(SIGUSR2, &sa, NULL);
	while (1)
		pause();
}
