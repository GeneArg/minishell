/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperez-a <bperez-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 10:11:15 by bperez-a          #+#    #+#             */
/*   Updated: 2024/05/17 09:12:54 by bperez-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void ft_error(t_program **program, char *cmd, char *error, int exit_status)
{
	char *exit_status_str;
	char *env_exit_status;

	exit_status_str = ft_itoa(exit_status);
	env_exit_status = ft_strjoin("?=", exit_status_str);
	free(exit_status_str);
	
	ft_putstr_fd("minishell: ", 2);
	if (cmd)
	{
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": ", 2);
	}
	ft_putstr_fd(error, 2);
	ft_putstr_fd("\n", 2);
	
	if (exit_status != -1)
		ft_export(program, (char *[]){"export", env_exit_status, NULL});
	free(env_exit_status);
}