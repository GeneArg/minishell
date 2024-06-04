/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperez-a <bperez-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 11:42:12 by eagranat          #+#    #+#             */
/*   Updated: 2024/06/04 13:40:29 by bperez-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	count_commands(t_command *current_command)
{
	int	num_commands;

	num_commands = 0;
	while (current_command)
	{
		num_commands++;
		current_command = current_command->next;
	}
	return (num_commands);
}

void	setup_pipes(int pipefds[2], t_command *current_command)
{
	if (current_command->next)
	{
		if (pipe(pipefds) == -1)
		{
			perror("pipe");
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		pipefds[0] = -1;
		pipefds[1] = -1;
	}
}

void	wait_for_children(int num_commands, t_program **program)
{
	int		status;
	int		exit_code;
	char	*exit_code_str;
	int		i;

	i = 0;
	while (i++ < num_commands)
	{
		wait(&status);
		if (WIFEXITED(status))
		{
			exit_code = WEXITSTATUS(status);
			exit_code_str = ft_itoa(exit_code);
			set_env_var(program, "?", exit_code_str);
			free(exit_code_str);
		}
	}
}

void	execute_command_in_pipeline(t_program **program,
		t_command *current_command, int num_commands)
{
	int		pipefds[2];
	pid_t	pid;
	int		i;

	i = 0;
	while (i++ < num_commands)
	{
		setup_pipes(pipefds, current_command);
		pid = fork();
		if (pid == 0)
			handle_child_process(program, current_command, pipefds);
		else if (pid > 0)
		{
			if ((*program)->in_fd != 0)
				close((*program)->in_fd);
			if (pipefds[1] != -1)
				close(pipefds[1]);
			(*program)->in_fd = pipefds[0];
		}
		current_command = current_command->next;
	}
}

void	execute_pipeline(t_program **program)
{
	t_command	*current_command;
	int			num_commands;

	current_command = (*program)->commands;
	num_commands = count_commands(current_command);
	(*program)->in_fd = 0;
	if (num_commands == 1 && current_command->argv[0]
		&& is_builtin(current_command->argv[0]))
	{
		(*program)->in_fd = 0;
		(*program)->out_fd = 1;
		execute_builtin_with_redirection(current_command, program);
		return ;
	}
	execute_command_in_pipeline(program, current_command, num_commands);
	if ((*program)->in_fd != 0)
		close((*program)->in_fd);
	wait_for_children(num_commands, program);
}
