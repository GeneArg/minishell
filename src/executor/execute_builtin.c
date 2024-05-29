/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperez-a <bperez-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 12:54:46 by bperez-a          #+#    #+#             */
/*   Updated: 2024/05/29 12:58:03 by bperez-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	execute_builtin_with_redirection(t_command *cmd, t_program **program,
		int in_fd, int out_fd)
{
	int		saved_stdout;
	int		saved_stdin;
	int		exit_code;
	char	*exit_code_str;
	char	*exit_code_env;

	saved_stdout = dup(1);
	saved_stdin = dup(0);
	exit_code = 0;
	if (saved_stdout == -1 || saved_stdin == -1)
	{
		perror("Failed to save STDIN or STDOUT");
		exit(EXIT_FAILURE);
	}
	dup2(in_fd, 0);
	dup2(out_fd, 1);
	if (!strcmp(cmd->argv[0], "export"))
		exit_code = ft_export(program, cmd->argv);
	else if (!strcmp(cmd->argv[0], "unset"))
		exit_code = ft_unset(program, cmd->argv);
	else if (!strcmp(cmd->argv[0], "echo"))
	{
		exit_code = ft_echo(cmd->argv);
	}
	else if (!ft_strncmp(cmd->argv[0], "pwd", 4))
		exit_code = ft_pwd();
	else if (!ft_strncmp(cmd->argv[0], "env", 4))
		exit_code = ft_env(program);
	else if (!strcmp(cmd->argv[0], "cd"))
		exit_code = ft_cd(program, cmd->argv);
	else if (!ft_strncmp(cmd->argv[0], "exit", 5))
		ft_exit(program, cmd);
	exit_code_str = ft_itoa(exit_code);
	exit_code_env = ft_strjoin("?=", exit_code_str);
	ft_export(program, (char *[]){"export", exit_code_env, NULL});
	free(exit_code_str);
	free(exit_code_env);
	dup2(saved_stdin, 0);
	dup2(saved_stdout, 1);
	close(saved_stdin);
	close(saved_stdout);
}
