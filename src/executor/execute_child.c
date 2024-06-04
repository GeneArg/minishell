/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_child.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperez-a <bperez-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 13:30:40 by bperez-a          #+#    #+#             */
/*   Updated: 2024/06/04 13:33:41 by bperez-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	setup_child_io(t_program **program, int pipefds[2])
{
	if ((*program)->in_fd != 0)
	{
		dup2((*program)->in_fd, 0);
		close((*program)->in_fd);
	}
	if (pipefds[1] != -1)
	{
		dup2(pipefds[1], 1);
		close(pipefds[1]);
	}
	if (pipefds[0] != -1)
		close(pipefds[0]);
}

void	execute_builtin_command(t_program **program, t_command *current_command)
{
	int	exit_code;

	(*program)->in_fd = 0;
	(*program)->out_fd = 1;
	execute_builtin_with_redirection(current_command, program);
	exit_code = ft_atoi(find_env_var_value((*program)->envp, "?"));
	exit(exit_code);
}

void	execute_external_command(t_program **program,
		t_command *current_command, char **env_copy)
{
	char	*cmd_path;
	int		execstat;

	cmd_path = find_path(env_copy, current_command->argv[0]);
	if (cmd_path)
	{
		if (current_command->argv[0][0] == '.'
			|| current_command->argv[0][0] == '/')
			check_access(cmd_path, current_command);
		execstat = execve(cmd_path, current_command->argv, env_copy);
		if (execstat == -1)
		{
			ft_error(program, current_command->argv[0], "command not found",
				COMMAND_NOT_FOUND);
			exit(COMMAND_NOT_FOUND);
		}
		free(cmd_path);
	}
	else
	{
		ft_error(program, current_command->argv[0], "command not found",
			COMMAND_NOT_FOUND);
		exit(COMMAND_NOT_FOUND);
	}
	exit(EXIT_FAILURE);
}

void	handle_child_process(t_program **program, t_command *current_command,
		int pipefds[2])
{
	char	**env_copy;

	env_copy = ft_copy_array((*program)->envp);
	setup_child_io(program, pipefds);
	handle_redirection(current_command);
	if (is_builtin(current_command->argv[0]))
		execute_builtin_command(program, current_command);
	else
		execute_external_command(program, current_command, env_copy);
}
