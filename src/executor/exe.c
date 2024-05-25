/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eagranat <eagranat@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 11:42:12 by eagranat          #+#    #+#             */
/*   Updated: 2024/05/26 00:06:16 by eagranat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Function declaration for set_env_var
void	set_env_var(t_program **program, char *key, char *value);
char	**ft_dup_array(char **array);
void	ft_free_array(char **array);

void	print_pids(pid_t *pids, int num_commands)
{
	for (int i = 0; i < num_commands; i++)
	{
		printf("pids[%d]: %d\n", i, pids[i]);
	}
}

bool	is_builtin(char *cmd)
{
	return (!strcmp(cmd, "cd") || !strcmp(cmd, "echo") || !strcmp(cmd, "export")
		|| !strcmp(cmd, "unset") || !strcmp(cmd, "pwd") || !strcmp(cmd, "env")
		|| !strcmp(cmd, "exit"));
}

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
		if (!cmd_path)
		{
			ft_error(program, cmd->argv[0], "command not found",
				COMMAND_NOT_FOUND);
			exit(COMMAND_NOT_FOUND);
		}
		if (cmd->argv[0][0] == '.' || cmd->argv[0][0] == '/')
			check_access(cmd_path, cmd);
		execstat = execve(cmd_path, cmd->argv, env_copy);
		if (execstat == -1)
		{
			perror("execve");
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

void	handle_open_error(t_program **program, char *file)
{
	char	*error_message;

	if (errno == ENOENT)
	{
		error_message = "No such file or directory";
	}
	else if (errno == EACCES)
	{
		error_message = "Permission denied";
	}
	else
	{
		error_message = strerror(errno);
	}
	ft_error(program, file, error_message, 1);
	ft_export(program, (char *[]){"export", "?=1", NULL});
}

void	execute_pipeline(t_program **program)
{
	t_command		*current_command;
	int				pipefds[2];
	int				in_fd;
	int				out_fd;
	pid_t			pid;
	int				status;
	int				num_commands;
	int				i;
	t_redirection	*current_redirection;
	int				fd;
	char			*cmd_path;
	int				exit_code;
			char exit_code_str[10];

	in_fd = 0;
	out_fd = 1;
	num_commands = 0;
	i = 0;
	current_command = (*program)->commands;
	while (current_command)
	{
		num_commands++;
		current_command = current_command->next;
	}
	current_command = (*program)->commands;
	for (i = 0; i < num_commands; i++)
	{
		char **env_copy = ft_dup_array((*program)->envp);
			// Create a copy of the environment variables
		if (current_command->next)
		{
			if (pipe(pipefds) == -1)
			{
				perror("pipe");
				exit(EXIT_FAILURE);
			}
			out_fd = pipefds[1];
		}
		else
		{
			out_fd = 1;
		}
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
			if (in_fd != 0)
				close(pipefds[0]);
			// Handle redirections
			current_redirection = current_command->redirects;
			while (current_redirection)
			{
				if (current_redirection->type == REDIRECT_IN)
				{
					fd = open(current_redirection->file, O_RDONLY);
					if (fd < 0)
					{
						perror(current_redirection->file);
						exit(EXIT_FAILURE);
					}
					dup2(fd, 0);
					close(fd);
				}
				else if (current_redirection->type == REDIRECT_OUT)
				{
					fd = open(current_redirection->file,
							current_command->append ? (O_WRONLY | O_CREAT | O_APPEND) : (O_WRONLY | O_CREAT | O_TRUNC),
							0644);
					if (fd < 0)
					{
						perror(current_redirection->file);
						exit(EXIT_FAILURE);
					}
					dup2(fd, 1);
					close(fd);
				}
				current_redirection = current_redirection->next;
			}
			if (is_builtin(current_command->argv[0]))
			{
				execute_builtin_with_redirection(current_command, program, 0,
					1);
				exit(0);
			}
			else
			{
				cmd_path = find_path(env_copy, current_command->argv[0]);
				if (cmd_path)
				{
					execve(cmd_path, current_command->argv, env_copy);
					free(cmd_path);
				}
				else
				{
					fprintf(stderr, "Command not found: %s\n",
						current_command->argv[0]);
				}
				perror("execve");
				exit(EXIT_FAILURE);
			}
		}
		else if (pid > 0)
		{ // Parent process
			if (in_fd != 0)
				close(in_fd);
			if (out_fd != 1)
				close(out_fd);
			in_fd = pipefds[0];
		}
		else
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		ft_free_array(env_copy); // Free the copied environment array
		current_command = current_command->next;
	}
	for (i = 0; i < num_commands; i++)
	{
		wait(&status);
		if (WIFEXITED(status))
		{
			exit_code = WEXITSTATUS(status);
			snprintf(exit_code_str, sizeof(exit_code_str), "%d", exit_code);
			set_env_var(program, "?", exit_code_str);
		}
	}
}

void	execute(t_program **program)
{
	t_command		*current_command;
	int				pipefds[2];
	t_redirection	*current_redirection;
	int				new_in_fd;
	int				new_out_fd;
	pid_t			*pids;
	int				i;
	int				num_commands;
		int status;
		char *exit_status_str;
		char *exit_code_env;

	if ((*program)->commands && (*program)->commands->next)
	{
		execute_pipeline(program);
		return ;
	}
	num_commands = 0;
	int in_fd = 0;  // Standard input by default
	int out_fd = 1; // Standard output by default
	current_command = (*program)->commands;
	while (current_command)
	{
		num_commands++;
		current_command = current_command->next;
	}
	pids = malloc(sizeof(pid_t) * num_commands);
	if (!pids)
	{
		ft_error(program, NULL, "Failed to allocate memory", FAILURE);
		exit(FAILURE);
	}
	current_command = (*program)->commands;
	i = 0;
	while (current_command)
	{
		if (!current_command->argv[0])
		{
			ft_export(program, (char *[]){"export", "?=0", NULL});
			current_command = current_command->next;
			pids[i] = -1;
			if (!current_command)
				return ;
			i++;
			continue ;
		}
		if (current_command->next)
		{
			if (pipe(pipefds) == -1)
			{
				ft_putstr_fd("Failed to create pipe\n", 2);
				pids[i] = -1;
				current_command = current_command->next;
				ft_export(program, (char *[]){"export", "?=1", NULL});
				i++;
				continue ;
			}
			out_fd = pipefds[1];
		}
		// Handle redirections
		current_redirection = current_command->redirects;
		while (current_redirection)
		{
			if (current_redirection->type == REDIRECT_IN)
			{
				new_in_fd = open(current_redirection->file, O_RDONLY);
				if (new_in_fd < 0)
				{
					handle_open_error(program, current_redirection->file);
					current_command->flag_error = 1;
					ft_export(program, (char *[]){"export", "?=1", NULL});
						// Set exit code to 1
					break ;
				}
				if (in_fd != 0)
					close(in_fd);
				in_fd = new_in_fd;
			}
			else if (current_redirection->type == REDIRECT_OUT)
			{
				new_out_fd = open(current_redirection->file,
						current_command->append ? (O_WRONLY | O_CREAT | O_APPEND) : (O_WRONLY | O_CREAT | O_TRUNC),
						0777);
				if (new_out_fd < 0)
				{
					handle_open_error(program, current_redirection->file);
					current_command->flag_error = 1;
					ft_export(program, (char *[]){"export", "?=1", NULL});
						// Set exit code to 1
					break ;
				}
				if (out_fd != 1)
				{
					close(out_fd);
				}
				out_fd = new_out_fd;
			}
			current_redirection = current_redirection->next;
			if (current_command->flag_error)
				break ;
		}
		if (current_command->flag_error)
		{
			pids[i] = -1;
			current_command = current_command->next;
			i++;
			if (in_fd != 0)
			{
				close(in_fd);
				in_fd = 0;
			}
			if (out_fd != 1)
			{
				close(out_fd);
				out_fd = 1;
			}
			continue ;
		}
		// Execute the command
		if (is_builtin(current_command->argv[0]))
		{
			execute_builtin_with_redirection(current_command, program, in_fd,
				out_fd);
			pids[i] = -1;
		}
		else
		{
			char **env_copy = ft_dup_array((*program)->envp);
				// Create a copy of the environment variables
			pids[i] = execute_in_child(current_command, program, in_fd, out_fd,
					env_copy);
			ft_free_array(env_copy); // Free the copied environment array
		}
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
		i++;
		current_command = current_command->next;
	}
	for (i = 0; i < num_commands; i++)
	{
		if (pids[i] == -1)
			continue ;
		waitpid(pids[i], &status, 0);
		if (WIFEXITED(status))
		{
			exit_status_str = ft_itoa(WEXITSTATUS(status));
			exit_code_env = ft_strjoin("?=", exit_status_str);
			ft_export(program, (char *[]){"export", exit_code_env, NULL});
			free(exit_status_str);
			free(exit_code_env);
		}
		if (WIFSIGNALED(status))
		{
			exit_status_str = ft_itoa(WTERMSIG(status) + 128);
			exit_code_env = ft_strjoin("?=", exit_status_str);
			ft_export(program, (char *[]){"export", exit_code_env, NULL});
			free(exit_status_str);
			free(exit_code_env);
		}
	}
	free(pids);
	// Ensure all file descriptors are closed
	if (in_fd != 0)
		close(in_fd);
	if (out_fd != 1)
		close(out_fd);
}
