/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperez-a <bperez-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 11:42:12 by eagranat          #+#    #+#             */
/*   Updated: 2024/05/10 10:24:52 by bperez-a         ###   ########.fr       */
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
	int		i;

	// char	**temp_envp;
	// temp_envp = envp;
	i = 0;
	while (envp[i] && !ft_strnstr(envp[i], "PATH=", 5))
		i++;
	sub = ft_substr(envp[i], 5, ft_strlen(envp[i]) - 5);
	paths = ft_split(sub, ':');
	free(sub);
	i = 0;
	while (envp[i] && !ft_strnstr(envp[i], "PWD=", 4))
		i++;
	pwd = ft_substr(envp[i], 4, ft_strlen(envp[i]) - 4);
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
	int		exit_code;
	int		i;

	is_digit = true;
	i = 0;
	ft_putstr_fd("exit\n", 1);
	if (ft_array_len(current_command->argv) == 1)
		free_program((*program), 0);
	else
	{
		if (current_command->argv[1][0] == '-'
			|| current_command->argv[1][0] == '+')
			i++;
		while (current_command->argv[1][i])
		{
			if (!isdigit(current_command->argv[1][i]))
			{
				is_digit = false;
				break ;
			}
			i++;
		}
		if (is_digit)
		{
			exit_code = ft_atoi(current_command->argv[1]);
			if (exit_code > 255)
				exit_code = exit_code % 256;
		}
		else
		{
			ft_putstr_fd("bash: exit: numeric argument required\n", 2);
			exit_code = 2;
		}
		i = 0;
		while (current_command->argv[++i])
		{
			if (i > 1)
			{
				ft_putstr_fd("bash: exit: too many arguments\n", 2);
				exit_code = 1;
				break ;
			}
		}
		free_program((*program), exit_code);
	}
}

int	ft_pwd(void)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		ft_putstr_fd("minishell: pwd: error retrieving current directory: getcwd: cannot access parent directories: No such file or directory\n",
			2);
		return (1);
	}
	ft_putstr_fd(pwd, 1);
	ft_putstr_fd("\n", 1);
	free(pwd);
	return (0);
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

void check_access(char *cmd_path, t_command *cmd) {
    struct stat statbuf;
    int stat_result = stat(cmd_path, &statbuf);

    if (stat_result == 0) {
        if (S_ISDIR(statbuf.st_mode)) {
            ft_putstr_fd("minishell: ", 2);
            ft_putstr_fd(cmd->argv[0], 2);
            ft_putstr_fd(": Is a directory\n", 2);
            exit(126);
        } else if (access(cmd_path, X_OK) != 0) {
            ft_putstr_fd("minishell: ", 2);
            ft_putstr_fd(cmd->argv[0], 2);
            ft_putstr_fd(": ", 2);
            ft_putstr_fd(strerror(errno), 2);
            ft_putstr_fd("\n", 2);
            exit(126);
        }
    } else {
        if (errno == EACCES) {
            ft_putstr_fd("minishell: ", 2);
            ft_putstr_fd(cmd->argv[0], 2);
            ft_putstr_fd(": No Permission denied\n", 2);
            exit(127);
        } else {
            ft_putstr_fd("minishell: ", 2);
            ft_putstr_fd(cmd->argv[0], 2);
            ft_putstr_fd(": No such file or directory\n", 2);
            exit(127);
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
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd->argv[0], 2);
			ft_putstr_fd(": command not found", 2);
			free(cmd_path);
			ft_export(program, (char *[]){"export", ft_strjoin("?=",
					ft_itoa(127)), NULL});
			exit(127);
		}
		free(cmd_path);
		exit(0);
	}
	else if (pid > 0)
	{ // Parent process
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
		{
			// printf("Program exited with status %d\n", WEXITSTATUS(status));
			ft_export(program, (char *[]){"export", ft_strjoin("?=",
					ft_itoa(WEXITSTATUS(status))), NULL});
		}
		if (WIFSIGNALED(status))
		{
			// printf("Program was killed by signal %d\n", WTERMSIG(status));
			// printf("Program exited with status %d\n", status);
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
				perror("Failed to open input file for redirection");
				exit(EXIT_FAILURE);
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
				perror("Failed to open output file for redirection");
				exit(EXIT_FAILURE);
			}
		}
		// Setup pipe if there is a next command
		if (current_command->next)
		{
			if (pipe(pipefds) == -1)
			{
				perror("Failed to create pipe");
				exit(EXIT_FAILURE);
			}
		}
		// Execute the command
		if (is_builtin(current_command->argv[0]))
			execute_builtin_with_redirection(current_command, program, in_fd,
				out_fd);
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
