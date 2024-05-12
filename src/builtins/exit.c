/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperez-a <bperez-a@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 10:36:36 by bperez-a          #+#    #+#             */
/*   Updated: 2024/05/12 20:40:19 by bperez-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

long long	mod_atoi(const char *str)
{
	long long	result;
	int			sign;

	result = 0;
	sign = 1;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str)
	{
		if (*str < '0' || *str > '9')
		{
			if (sign == 1)
				return (-1);
			else
				return (0);
		}
		if (sign == 1 && result > (9223372036854775807LL - (*str - '0')) / 10)
			return (-1);
		else if (sign == -1
			&& (unsigned long long)result > (9223372036854775808ULL - (*str
					- '0')) / 10)
			return (-1);
		result = result * 10 + (*str - '0');
		str++;
	}
	return (result * sign);
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
		free_program((*program), 0);
	else
	{
		if (current_command->argv[1][0] == '-'
			|| current_command->argv[1][0] == '+')
			i++;
		while (current_command->argv[1][i])
		{
			if (!ft_isdigit(current_command->argv[1][i]))
			{
				is_digit = false;
				break ;
			}
			i++;
		}
		if (is_digit)
		{
			exit_code = mod_atoi(current_command->argv[1]);
			if (exit_code == -1)
			{
				ft_putstr_fd("bash: exit: ", 2);
				ft_putstr_fd(current_command->argv[1], 2);
				ft_putstr_fd(": numeric argument required\n", 2);
				exit_code = 2;
			}
			else if (exit_code > 255 || exit_code < -255)
				exit_code = exit_code % 256;
		}
		else
		{
			ft_putstr_fd("bash: exit: numeric argument required\n", 2);
			exit_code = 2;
		}
		i = 0;
		while (current_command->argv[++i])
		{
			if (i > 1)
			{
				ft_putstr_fd("bash: exit: too many arguments\n", 2);
				exit_code = 1;
				break ;
			}
		}
		free_program((*program), exit_code);
	}
}
