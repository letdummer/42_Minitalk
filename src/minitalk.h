/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minitalk.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldummer- <ldummer-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 18:18:59 by ldummer-          #+#    #+#             */
/*   Updated: 2025/07/01 00:30:05 by ldummer-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINITALK_H
# define MINITALK_H

# include <signal.h>
# include <unistd.h>	
# include "../libft/libft/libft.h"
# include "../libft/ft_printf/libftprintf.h"

typedef struct s_client
{
	int		pid;
	char	*message;
	size_t	message_length;
	int		index;
}	t_client;

#endif