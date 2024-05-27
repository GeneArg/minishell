/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperez-a <bperez-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 10:36:36 by bperez-a          #+#    #+#             */
/*   Updated: 2024/05/27 14:16:57 by bperez-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

long long	mod_atoi(const char *str)
{
	long long	result;
	int			sign;

	result = 0;
	sign = 1;
	if ((*str == '-' || *str == '+') && (*str++ == '-'))
		sign = -1;
	while (*str)
	{
		if (*str < '0' || *str > '9')
		{
			if (sign == 1)
				return (-1);
			else
				return (0);
		}
		if ((sign == 1 && result > (9223372036854775807LL - (*str - '0')) / 10)
			|| (sign == -1
				&& (unsigned long long)result > (9223372036854775808ULL - (*str
						- '0')) / 10))
			return (-1);
		result = result * 10 + (*str++ - '0');
	}
	return (result * sign);
}

long long	set_exit_code(t_program **program, t_command *current_command,
		bool is_digit)
{
	long long	exit_code;

	if (is_digit)
	{
		exit_code = mod_atoi(current_command->argv[1]);
		if (exit_code == -1)
		{
			ft_error(program, "exit", "numeric argument required", -1);
			exit_code = INVALID_ARGS;
		}
		else if (exit_code > 255 || exit_code < -255)
			exit_code = exit_code % 256;
	}
	else
	{
		ft_error(program, "exit", "numeric argument required", -1);
		exit_code = INVALID_ARGS;
	}
	if (current_command->argv[1] && current_command->argv[2])
	{
		ft_error(program, "exit", "too many arguments", -1);
		exit_code = FAILURE;
	}
	return (exit_code);
}

void	ft_exit(t_program **program, t_command *current_command)
{
	bool		is_digit;
	long long	exit_code;
	int			i;

	is_digit = true;
	i = 0;
	ft_putstr_fd("exit\n", 1);
	if (ft_array_len(current_command->argv) == 1)
		free_and_exit((*program), 0);
	else
	{
		if (current_command->argv[1][0] == '-'
			|| current_command->argv[1][0] == '+')
			i++;
		while (current_command->argv[1][i])
		{
			if (!ft_isdigit(current_command->argv[1][i++]))
			{
				is_digit = false;
				break ;
			}
		}
		exit_code = set_exit_code(program, current_command, is_digit);
		free_and_exit((*program), exit_code);
	}
}
