/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_child.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperez-a <bperez-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 12:56:06 by bperez-a          #+#    #+#             */
/*   Updated: 2024/05/29 12:56:10 by bperez-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

pid_t	execute_in_child(t_command *cmd, t_program **program, int in_fd,
		int out_fd, char **env_copy)
{
	char	*cmd_path;
	pid_t	pid;
	int		execstat;

	pid = fork();
	if (pid == 0)
	{ // Child process
		if (in_fd != 0)
		{
			dup2(in_fd, 0);
			close(in_fd);
		}
		if (out_fd != 1)
		{
			dup2(out_fd, 1);
			close(out_fd);
		}
		if (cmd->argv[0][0] == '/')
			cmd_path = ft_strdup(cmd->argv[0]);
		else
			cmd_path = find_path(env_copy, cmd->argv[0]);
		if (cmd->argv[0][0] == '.' || cmd->argv[0][0] == '/')
			check_access(cmd_path, cmd);
		execstat = execve(cmd_path, cmd->argv, env_copy);
		if (execstat == -1)
		{
			ft_error(program, cmd->argv[0], "command not found",
				COMMAND_NOT_FOUND);
			exit(COMMAND_NOT_FOUND);
		}
		free(cmd_path);
		exit(SUCCESS);
	}
	else if (pid > 0)
	{ // Parent process
		if (in_fd != 0)
			close(in_fd);
		if (out_fd != 1)
			close(out_fd);
	}
	else
	{
		ft_error(program, cmd->argv[0], "Failed to fork process", FAILURE);
		exit(FAILURE);
	}
	return (pid);
}
