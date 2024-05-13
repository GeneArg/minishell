/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperez-a <bperez-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 11:42:12 by eagranat          #+#    #+#             */
/*   Updated: 2024/05/13 11:10:37 by bperez-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"






bool	is_builtin(char *cmd)
{
	return (!strcmp(cmd, "cd") || !strcmp(cmd, "echo") || !strcmp(cmd, "export")
		|| !strcmp(cmd, "unset") || !strcmp(cmd, "pwd") || !strcmp(cmd, "env")
		|| !strcmp(cmd, "exit"));
}





void	execute_builtin_with_redirection(t_command *cmd, t_program **program,
		int in_fd, int out_fd)
{
	int	saved_stdout;
	int	saved_stdin;
	int	exit_code;

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
		exit_code = ft_echo(cmd->argv);
	else if (!ft_strncmp(cmd->argv[0], "pwd", 4))
		exit_code = ft_pwd();
	else if (!ft_strncmp(cmd->argv[0], "env", 4))
		exit_code = ft_env(program);
	else if (!strcmp(cmd->argv[0], "cd"))
		exit_code = ft_cd(program, cmd->argv);
	else if (!ft_strncmp(cmd->argv[0], "exit", 5))
		ft_exit(program, cmd);
	ft_export(program, (char *[]){"export", ft_strjoin("?=",
			ft_itoa(exit_code)), NULL});
	// Restore STDIN and STDOUT
	dup2(saved_stdin, 0);
	dup2(saved_stdout, 1);
	close(saved_stdin);
	close(saved_stdout);
}

void	check_access(char *cmd_path, t_command *cmd)
{
	struct stat	statbuf;
	int			stat_result;

	stat_result = stat(cmd_path, &statbuf);
	if (stat_result == 0)
	{
		if (S_ISDIR(statbuf.st_mode))
		{
			ft_error(NULL, cmd->argv[0], "Is a directory", -1);
			exit(CANNOT_EXECUTE);
		}
		else if (access(cmd_path, X_OK) != 0)
		{
			ft_error(NULL, cmd->argv[0], "Permission denied", -1);
			exit(CANNOT_EXECUTE);
		}
	}
	else
	{
		if (errno == EACCES)
		{
			ft_error(NULL, cmd->argv[0], "Permission denied", -1);
			exit(COMMAND_NOT_FOUND);
		}
		else
		{
			ft_error(NULL, cmd->argv[0], "No such file or directory", -1);
			exit(COMMAND_NOT_FOUND);
		}
	}
}

void	execute_in_child(t_command *cmd, t_program **program, int in_fd,
		int out_fd)
{
	char	*cmd_path;
	pid_t	pid;
	int		execstat;
	int		status;
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
			cmd_path = find_path((*program)->envp, cmd->argv[0]);
		if (cmd->argv[0][0] == '.' || cmd->argv[0][0] == '/')
			check_access(cmd_path, cmd);
		execstat = execve(cmd_path, cmd->argv, (*program)->envp);
		if (execstat == -1)
		{
			ft_error(program, cmd->argv[0], ": command not found\n", COMMAND_NOT_FOUND);
			exit(COMMAND_NOT_FOUND);
		}
		free(cmd_path);
		exit(SUCCESS);
	}
	else if (pid > 0)
	{ // Parent process
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
		{
			ft_export(program, (char *[]){"export", ft_strjoin("?=",
					ft_itoa(WEXITSTATUS(status))), NULL});
		}
		if (WIFSIGNALED(status))
		{
			ft_export(program, (char *[]){"export", ft_strjoin("?=",
					ft_itoa(status + 128)), NULL});
		}
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
}




void	execute(t_program **program)
{
	t_command		*current_command;
	int				pipefds[2];
	t_redirection	*current_redirection;
	int				new_in_fd;

	current_command = (*program)->commands;
	int in_fd = 0;  // Standard input by default
	int out_fd = 1; // Standard output by default
	while (current_command)
	{
		if (!current_command->argv[0])
		{
			current_command = current_command->next;
			continue ;
		}
		// Handle input redirections
		current_redirection = current_command->redirect_in;
		while (current_redirection)
		{
			new_in_fd = open(current_redirection->file, O_RDONLY);
			if (new_in_fd < 0)
			{
				ft_error(program, current_redirection->file, "No such file or directory", 1);
				current_command->flag_error = 1;
				break;
			}
			if (in_fd != 0)
			{
				close(in_fd);
			}
			in_fd = new_in_fd;
			current_redirection = current_redirection->next;
		}
		// Handle output redirection
		if (current_command->redirect_out)
		{
			out_fd = open(current_command->redirect_out,
					current_command->append ? (O_WRONLY | O_CREAT | O_APPEND) : (O_WRONLY | O_CREAT | O_TRUNC),
					0777);
			if (out_fd < 0)
			{
				ft_putstr_fd("Permission denied\n", 2);
				current_command = current_command->next;
				continue ;
			}
		}
		if (current_command->flag_error)
		{
			current_command = current_command->next;
			continue ;
		}
		// Setup pipe if there is a next command
		if (current_command->next)
		{
			if (pipe(pipefds) == -1)
			{
				ft_putstr_fd("Failed to create pipe\n", 2);
				current_command = current_command->next;
				continue ;
			}
			out_fd = pipefds[1];
		}
		// Execute the command
		if (is_builtin(current_command->argv[0]))
		{
			execute_builtin_with_redirection(current_command, program, in_fd,
				out_fd);
		}
		else
			execute_in_child(current_command, program, in_fd, out_fd);
		// If there was output redirection, close the output file
		if (out_fd != 1)
		{
			close(out_fd);
			out_fd = 1;
		}
		// If there was a pipe,
		// close the read end and make the write end the new input
		if (current_command->next)
		{
			close(pipefds[1]);
			in_fd = pipefds[0];
		}
		current_command = current_command->next;
	}
}
