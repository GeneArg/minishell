/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperez-a <bperez-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 10:11:15 by bperez-a          #+#    #+#             */
/*   Updated: 2024/06/04 11:29:01 by bperez-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_error(t_program **program, char *cmd, char *error, int exit_status)
{
	char	*exit_status_str;
	char	*env_exit_status;

	exit_status_str = ft_itoa(exit_status);
	env_exit_status = ft_strjoin("?=", exit_status_str);
	free(exit_status_str);
	ft_putstr_fd("minishell: ", 2);
	if (cmd)
	{
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": ", 2);
	}
	if (error)
		ft_putstr_fd(error, 2);
	ft_putstr_fd("\n", 2);
	if (exit_status != -1)
		ft_export(program, (char *[]){"export", env_exit_status, NULL});
	free(env_exit_status);
}

void	handle_open_error(t_program **program, char *file)
{
	char	*error_message;

	if (errno == ENOENT)
		error_message = "No such file or directory";
	else if (errno == EACCES)
		error_message = "Permission denied";
	else
		error_message = strerror(errno);
	ft_error(program, file, error_message, 1);
	ft_export(program, (char *[]){"export", "?=1", NULL});
}
