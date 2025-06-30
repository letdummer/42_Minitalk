#include "minitalk.h"
// if SIGUSR1 => 1
// if SIGUSR2 => 0

void	handler(int signum, siginfo_t *info, void *context)
{
	static int		bit_count;
	static char		c;
	static char		*message = NULL;
	static size_t	index;
	static size_t	message_length;
	static int		receiving_length = 1;
	static size_t	length_bits_received;

	
	(void)context;
	(void)info;
	if (receiving_length)
	{
		if (signum == SIGUSR1)
			message_length |= (1UL << length_bits_received);
		length_bits_received++;
		if (length_bits_received == sizeof(size_t) * 8)
		{
			receiving_length = 0;
			message = malloc(message_length + 1);
			ft_printf("trying to allocate  %d characters\n", message_length +1 );
		if (!message)
		{
			ft_printf("Memory allocation failed l = %d\n", message_length +1);
			receiving_length = 1;
			message_length = 0;
			length_bits_received = 0;
			return;
		}
			message[message_length] = '\0';
		}
		return;
	}
	if (signum == SIGUSR1)
		c = c | (1 << bit_count);
	bit_count++;
	if (bit_count == 8)
	{
		message[index++] = c;
		if (c == '\0')
		{
			ft_putstr_fd(message, 1);
			ft_putchar_fd('\n', 1);
			free(message);
			message = NULL;
			index = 0;
			receiving_length = 1;
			message_length = 0;
			length_bits_received = 0;
			kill(info->si_pid, SIGUSR1);

		}
		bit_count = 0;
		c = 0;
	}
	
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
	// mantem o servidor rodando para receber sinais
	while(1)
		pause();
}




/* The sigaction structure is defined as something like:

struct sigaction {
	void	 (*sa_handler)(int);
	void	 (*sa_sigaction)(int, siginfo_t *, void *);
	sigset_t   sa_mask;
	int		sa_flags;
	void	 (*sa_restorer)(void);
}; */

