/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_child.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperez-a <bperez-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 12:56:06 by bperez-a          #+#    #+#             */
/*   Updated: 2024/06/04 11:28:45 by bperez-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	handle_fd(int fd, int std_fd)
{
	if (fd != std_fd)
	{
		dup2(fd, std_fd);
		close(fd);
	}
}

void	check_and_execute(t_command *cmd, char *cmd_path, char **env_copy,
		t_program **program)
{
	int	execstat;

	if (cmd->argv[0][0] == '.' || cmd->argv[0][0] == '/')
		check_access(cmd_path, cmd);
	execstat = execve(cmd_path, cmd->argv, env_copy);
	if (execstat == -1)
	{
		ft_error(program, cmd->argv[0], "command not found", COMMAND_NOT_FOUND);
		exit(COMMAND_NOT_FOUND);
	}
}

void	handle_child_process(t_command *cmd, t_program **program,
		char **env_copy)
{
	char	*cmd_path;

	handle_fd((*program)->in_fd, 0);
	handle_fd((*program)->out_fd, 1);
	if (cmd->argv[0][0] == '/')
		cmd_path = ft_strdup(cmd->argv[0]);
	else
		cmd_path = find_path(env_copy, cmd->argv[0]);
	check_and_execute(cmd, cmd_path, env_copy, program);
	free(cmd_path);
	exit(SUCCESS);
}

void	handle_parent_process(int in_fd, int out_fd)
{
	if (in_fd != 0)
		close(in_fd);
	if (out_fd != 1)
		close(out_fd);
}

pid_t	execute_in_child(t_command *cmd, t_program **program, char **env_copy)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
		handle_child_process(cmd, program, env_copy);
	else if (pid > 0)
		handle_parent_process((*program)->in_fd, (*program)->out_fd);
	else
	{
		ft_error(program, cmd->argv[0], "Failed to fork process", FAILURE);
		exit(FAILURE);
	}
	return (pid);
}
