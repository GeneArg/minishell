/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_child.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eagranat <eagranat@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 12:56:06 by bperez-a          #+#    #+#             */
/*   Updated: 2024/06/01 16:20:55 by eagranat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	handle_in_fd(int in_fd)
{
	if (in_fd != 0)
	{
		dup2(in_fd, 0);
		close(in_fd);
	}
}

void	handle_out_fd(int out_fd)
{
	if (out_fd != 1)
	{
		dup2(out_fd, 1);
		close(out_fd);
	}
}

char	*get_command_path(t_command *cmd, char **env_copy)
{
	if (cmd->argv[0][0] == '/')
		return (ft_strdup(cmd->argv[0]));
	return (find_path(env_copy, cmd->argv[0]));
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

void	execute_command(t_command *cmd, char *cmd_path, char **env_copy,
		t_program **program)
{
	check_and_execute(cmd, cmd_path, env_copy, program);
}

void	handle_child_process_fds(int in_fd, int out_fd)
{
	handle_in_fd(in_fd);
	handle_out_fd(out_fd);
}

void	handle_child_process(t_command *cmd, t_program **program, int in_fd,
		int out_fd, char **env_copy)
{
	char	*cmd_path;

	cmd_path = get_command_path(cmd, env_copy);
	handle_child_process_fds(in_fd, out_fd);
	execute_command(cmd, cmd_path, env_copy, program);
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

void	handle_fork_error(t_program **program, char *cmd)
{
	ft_error(program, cmd, "Failed to fork process", FAILURE);
	exit(FAILURE);
}

pid_t	execute_in_child(t_command *cmd, t_program **program, int in_fd,
		int out_fd, char **env_copy)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		handle_child_process(cmd, program, in_fd, out_fd, env_copy);
	}
	else if (pid > 0)
	{
		handle_parent_process(in_fd, out_fd);
	}
	else
	{
		handle_fork_error(program, cmd->argv[0]);
	}
	return (pid);
}
