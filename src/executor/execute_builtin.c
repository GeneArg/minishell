/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperez-a <bperez-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 12:54:46 by bperez-a          #+#    #+#             */
/*   Updated: 2024/06/04 13:19:02 by bperez-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	save_and_redirect_fds(int *saved_stdin, int *saved_stdout, int in_fd,
		int out_fd)
{
	*saved_stdout = dup(1);
	*saved_stdin = dup(0);
	if (*saved_stdout == -1 || *saved_stdin == -1)
	{
		perror("Failed to save STDIN or STDOUT");
		exit(EXIT_FAILURE);
	}
	dup2(in_fd, 0);
	dup2(out_fd, 1);
}

void	restore_fds(int saved_stdin, int saved_stdout)
{
	dup2(saved_stdin, 0);
	dup2(saved_stdout, 1);
	close(saved_stdin);
	close(saved_stdout);
}

int	execute_builtin(t_command *cmd, t_program **program)
{
	int	exit_code;

	exit_code = 0;
	if (!strcmp(cmd->argv[0], "export"))
		exit_code = ft_export(program, cmd->argv);
	else if (!strcmp(cmd->argv[0], "unset"))
		exit_code = ft_unset(program, cmd->argv);
	else if (!strcmp(cmd->argv[0], "echo"))
		exit_code = ft_echo(cmd->argv);
	else if (!ft_strncmp(cmd->argv[0], "pwd", 4))
		exit_code = ft_pwd();
	else if (!ft_strncmp(cmd->argv[0], "env", 4))
		exit_code = ft_env(program);
	else if (!strcmp(cmd->argv[0], "cd"))
		exit_code = ft_cd(program, cmd->argv);
	else if (!ft_strncmp(cmd->argv[0], "exit", 5))
		ft_exit(program, cmd);
	return (exit_code);
}

void	set_exit_code_env(t_program **program, int exit_code)
{
	char	*exit_code_str;
	char	*exit_code_env;

	exit_code_str = ft_itoa(exit_code);
	exit_code_env = ft_strjoin("?=", exit_code_str);
	ft_export(program, (char *[]){"export", exit_code_env, NULL});
	free(exit_code_str);
	free(exit_code_env);
}

void	execute_builtin_with_redirection(t_command *cmd, t_program **program)
{
	int	saved_stdout;
	int	saved_stdin;
	int	exit_code;

	save_and_redirect_fds(&saved_stdin, &saved_stdout, (*program)->in_fd,
		(*program)->out_fd);
	handle_redirection(cmd);
	exit_code = execute_builtin(cmd, program);
	set_exit_code_env(program, exit_code);
	restore_fds(saved_stdin, saved_stdout);
}
