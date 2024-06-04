/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperez-a <bperez-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 12:52:08 by bperez-a          #+#    #+#             */
/*   Updated: 2024/06/04 13:01:55 by bperez-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

bool	is_builtin(char *cmd)
{
	return (!strcmp(cmd, "cd") || !strcmp(cmd, "echo") || !strcmp(cmd, "export")
		|| !strcmp(cmd, "unset") || !strcmp(cmd, "pwd") || !strcmp(cmd, "env")
		|| !strcmp(cmd, "exit"));
}

void	check_access(char *cmd_path, t_command *cmd)
{
	struct stat	statbuf;
	int			stat_result;

	stat_result = stat(cmd_path, &statbuf);
	if (S_ISDIR(statbuf.st_mode) && stat_result == 0)
	{
		ft_error(NULL, cmd->argv[0], "Is a directory", -1);
		exit(CANNOT_EXECUTE);
	}
	else if (access(cmd_path, X_OK) != 0 && stat_result == 0)
	{
		ft_error(NULL, cmd->argv[0], "Permission denied", -1);
		exit(CANNOT_EXECUTE);
	}
	if (errno == EACCES)
	{
		ft_error(NULL, cmd->argv[0], "Permission denied", -1);
		exit(COMMAND_NOT_FOUND);
	}
	else if (errno == ENOENT)
	{
		ft_error(NULL, cmd->argv[0], "No such file or directory", -1);
		exit(COMMAND_NOT_FOUND);
	}
}
