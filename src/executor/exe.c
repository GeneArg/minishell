/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eagranat <eagranat@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 11:42:12 by eagranat          #+#    #+#             */
/*   Updated: 2024/05/06 17:22:53 by eagranat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	free_split(char **split)
{
	char	**tmp;

	tmp = split;
	while (*split)
	{
		free(*split);
		split++;
	}
	free(tmp);
}

void	append_str_to_array(char ***array, char *str)
{
	int		len;
	char	**new_array;

	len = 0;
	if (**array)
	{
		while ((*array)[len])
			len++;
	}
	new_array = (char **)malloc(sizeof(char *) * (len + 2));
	for (int i = 0; i < len; i++)
	{
		new_array[i] = (*array)[i];
		(*array)[i] = NULL;
	}
	new_array[len] = ft_strdup(str);
	new_array[len + 1] = NULL;
	// if (*array)
	//     free_split(*array);
	*array = new_array;
}

char	**get_paths(char **envp)
{
	char	*sub;
	char	**paths;
	char	*pwd;

	// char	**temp_envp;
	// temp_envp = envp;
	while (*envp && !ft_strnstr(*envp, "PATH=", 5))
		envp++;
	sub = ft_substr(*envp, 5, ft_strlen(*envp) - 5);
	paths = ft_split(sub, ':');
	free(sub);
	while (*envp && !ft_strnstr(*envp, "PWD=", 4))
		envp++;
	pwd = ft_substr(*envp, 4, ft_strlen(*envp) - 4);
	// for (int i = 0; paths[i]; i++)
	// 	printf("paths[%d]: %s\n", i, paths[i]);
	append_str_to_array(&paths, pwd);
	// printf("pwd: %s\n", pwd);
	return (paths);
}

char	*find_path(char **envp, char *cmd)
{
	char	**paths;
	char	*cmd_path;
	char	*sub;
	char	**temp_paths;

	// char	**cmd_split;
	// cmd_split = ft_split(cmd, ' ');
	// cmd = *cmd_split;
	paths = get_paths(envp);
	temp_paths = paths;
	sub = ft_strjoin("/", cmd);
	// printf("cmd: %s\n", cmd);
	while (*temp_paths)
	{
		cmd_path = ft_strjoin(*temp_paths, sub);
		// printf("cmd_path: %s\n", cmd_path);
		if (!access(cmd_path, F_OK))
			break ;
		free(cmd_path);
		cmd_path = NULL;
		temp_paths++;
	}
	free(sub);
	// free_split(cmd_split);
	free_split(paths);
	return (cmd_path);
}

int	find_env_var(char **envp, char *var)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], var, ft_strlen(var)))
			return (i);
		i++;
	}
	return (-1);
}

char	*find_env_var_value(char **envp, char *var)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], var, ft_strlen(var)))
			return (ft_strchr(envp[i], '=') + 1);
		i++;
	}
	return (NULL);
}

bool	is_builtin(char *cmd)
{
	return (!strcmp(cmd, "cd") || !strcmp(cmd, "echo") || !strcmp(cmd, "export")
		|| !strcmp(cmd, "unset") || !strcmp(cmd, "pwd") || !strcmp(cmd, "env")
		|| !strcmp(cmd, "exit"));
}

void	ft_exit(t_program **program, t_command *current_command)
{
	bool	is_digit;
	int		i;

	ft_putstr_fd("exit\n", 1);
	if (ft_array_len(current_command->argv) == 1)
		free_program((*program), 0);
	for (int i = 0; current_command->argv[1][i]; i++)
	{
		if (!isdigit(current_command->argv[1][i]))
		{
			is_digit = false;
			break ;
		}
		is_digit = true;
	}
	if (is_digit)
		i = ft_atoi(current_command->argv[1]);
	printf("array len : %zu\n", ft_array_len(current_command->argv));
	printf("is_digit : %d\n", is_digit);
	if (ft_array_len(current_command->argv) > 2 && is_digit)
	{
		ft_putstr_fd("bash: exit: too many arguments\n", 2);
		// set exit code to 1 without exiting
	}
	else if (current_command->argv[1] && !is_digit)
	{
		ft_putstrs_fd("bash: exit: ", current_command->argv[1],
			": numeric argument required\n", 2);
		free_program((*program), 2);
	}
	else if (current_command->argv[1] && is_digit)
		free_program((*program), i % 256);
	else
	{
		if (!is_digit)
		{
			ft_putstrs_fd("bash: exit: ", current_command->argv[1],
				": numeric argument required\n", 2);
			free_program((*program), 2);
		}
		free_program((*program), 0);
	}
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
		printf("%s\n", getcwd(NULL, 0));
	else if (!ft_strncmp(cmd->argv[0], "env", 4))
	{
		for (int i = 0; (*program)->envp[i]; i++)
		{
			if (ft_strncmp((*program)->envp[i], "_=", 2))
				printf("%s\n", (*program)->envp[i]);
		}
	}
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

void	execute_in_child(t_command *cmd, t_program **program, int in_fd,
		int out_fd)
{
	char	*cmd_path;
	pid_t	pid;
	int		execstat;
		int status;

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
		execstat = execve(cmd_path, cmd->argv, (*program)->envp);
		if (execstat == -1)
		{
			printf("minishell: %s: command not found\n", cmd->argv[0]);
			free(cmd_path);
			exit(EXIT_FAILURE);
		}
		free(cmd_path);
		exit(0);
	}
	else if (pid > 0)
	{ // Parent process
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
		{
			printf("Program exited with status %d\n", WEXITSTATUS(status));
			ft_export(program, (char *[]){"export", ft_strjoin("?=",
					ft_itoa(WEXITSTATUS(status))), NULL});
		}
		if (WIFSIGNALED(status))
		{
			printf("Program was killed by signal %d\n", WTERMSIG(status));
			printf("Program exited with status %d\n", status);
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
		perror("fork failed");
		exit(EXIT_FAILURE);
	}
}

void	execute(t_program **program)
{
	t_command	*current_command;
	int			pipefds[2];

	current_command = (*program)->commands;
	int in_fd = 0;  // Standard input by default
	int out_fd = 1; // Standard output by default
	while (current_command)
	{
		// Handle output redirection
		if (current_command->redirect_out)
		{
			out_fd = open(current_command->redirect_out,
					current_command->append ? (O_WRONLY | O_CREAT | O_APPEND) : (O_WRONLY | O_CREAT | O_TRUNC),
					0777);
			if (out_fd < 0)
			{
				perror("Failed to open output file for redirection");
				exit(EXIT_FAILURE);
			}
		}
		// Setup pipe if there is a next command
		if (current_command->next)
		{
			if (pipe(pipefds) == -1)
			{
				perror("pipe");
				exit(1);
			}
			out_fd = pipefds[1];
		}
		// Determine if command is a built-in
		if (is_builtin(current_command->argv[0]))
		{
			execute_builtin_with_redirection(current_command, program, in_fd,
				out_fd);
		}
		else
		{
			execute_in_child(current_command, program, in_fd, out_fd);
		}
		// Manage file descriptors for the next command
		if (current_command->next)
		{
			close(pipefds[1]); // Close the write end of the pipe in the parent
			if (in_fd != 0)
				close(in_fd);   // Close the old in_fd if it was redirected
			in_fd = pipefds[0]; // Next command reads from here
		}
		else if (out_fd != 1)
		{
			close(out_fd);
		}
		current_command = current_command->next;
	}
}
