/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperez-a <bperez-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 10:25:50 by eagranat          #+#    #+#             */
/*   Updated: 2024/05/06 09:34:06 by bperez-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

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
	// char	**temp_envp;

	// temp_envp = envp;

	while (*envp && !ft_strnstr(*envp, "PATH=", 5))
		envp++;
	sub = ft_substr(*envp, 5, ft_strlen(*envp) - 5);
	paths = ft_split(sub, ':');
	free(sub);
	while (*envp && !ft_strnstr(*envp, "PWD=", 4))
		envp++;
	char *pwd = ft_substr(*envp, 4, ft_strlen(*envp) - 4);
	// for (int i = 0; paths[i]; i++)
	// 	printf("paths[%d]: %s\n", i, paths[i]);

	append_str_to_array(&paths, pwd);
	//printf("pwd: %s\n", pwd);
	return (paths);
}

char	*find_path(char **envp, char *cmd)
{
	char	**paths;
	char	*cmd_path;
	char	*sub;
	//char	**cmd_split;
	char	**temp_paths;

	//cmd_split = ft_split(cmd, ' ');
	
	//cmd = *cmd_split;
	paths = get_paths(envp);
	temp_paths = paths;
	sub = ft_strjoin("/", cmd);
	//printf("cmd: %s\n", cmd);
	while (*temp_paths)
	{
		cmd_path = ft_strjoin(*temp_paths, sub);
		//printf("cmd_path: %s\n", cmd_path);
		if (!access(cmd_path, F_OK))
			break ;
		free(cmd_path);
		cmd_path = NULL;
		temp_paths++;
	}
	free(sub);
	//free_split(cmd_split);
	free_split(paths);
	return (cmd_path);
}


int	find_env_var(char **envp, char *var)
{
	int i;

	i = 0;
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], var, ft_strlen(var)))
			return (i);
		i++;
	}
	return (-1);
}

char *find_env_var_value(char **envp, char *var)
{
	int i;

	i = 0;
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], var, ft_strlen(var)))
			return (ft_strchr(envp[i], '=') + 1);
		i++;
	}
	return (NULL);
}

void	ft_cd(char *path, t_program *program)
{
	(void)program;
	if (chdir(path) == -1)
	{
		printf("minishell: cd: %s: No such file or directory\n", path);
	}
	else
	{
		char *cwd = getcwd(NULL, 0);
		char *oldpwd = ft_strjoin("OLDPWD=", find_env_var_value(program->envp, "PWD"));
		program->envp[find_env_var(program->envp, "PWD")] = ft_strjoin("PWD=", cwd);
		program->envp[find_env_var(program->envp, "OLDPWD")] = oldpwd;
		free(cwd);
	}
}

char *ft_strip_quotes(char *str)
{
	int len = 0;
	int i = 0;
	char *new_str;
	while (str[i])
	{
		if (str[i] != '\'' && str[i] != '\"')
			len++;
		i++;
	}
	new_str = (char *)malloc(sizeof(char) * (len + 1));
	i = 0;
	int j = 0;

	while (str[i])
	{
		if (str[i] != '\'' && str[i] != '\"')
		{
			new_str[j] = str[i];
			j++;
		}
		i++;
	}
	new_str[j] = '\0';
	return (new_str);
}

// int check_builtins(t_program *program)
// {
// 	program->commands->ret = 0;
// 	if (!check_builts(program))
// 		return (program->commands->ret);
// 	else if (!ft_strncmp(program->commands->argv[0], "env", 4))
// 		env_command(program);
// 	else if (!ft_strncmp(program->commands->argv[0], "./", 2) ||
// 			!ft_strncmp(program->commands->argv[0], "../", 3) ||
// 			!ft_strncmp(program->commands->argv[0], "/", 1))
// 		bash_command(program);
// 	else if (!ft_strncmp(program->commands->argv[0], "export", 7) ||
// 			!ft_strncmp(program->commands->argv[0], "unset", 6))
// 		program->envp = 
// }

void execute(t_program **program)
{
	t_command *current_command;
	//int status;
	int fd[2];
	int in;
	int out;
	char *cmd_path;

	current_command = (*program)->commands;
	in = 0;
	while (current_command)
	{
		if (current_command->next)
		{
			if (pipe(fd) == -1)
				exit(1);
		}
		else
			fd[1] = 1;
		if (current_command->redirect_in)
		{
			in = open(current_command->redirect_in, O_RDONLY);
			if (in < 0)
				exit(1);
		}
		if (current_command->redirect_out)
		{
			if (current_command->append)
				out = open(current_command->redirect_out, O_WRONLY | O_CREAT | O_APPEND, 0777);
			else
				out = open(current_command->redirect_out, O_WRONLY | O_CREAT | O_TRUNC, 0777);
			if (out < 0)
				exit(1);
		}
		
		//check_builtin(program);
		// if (current_command->heredoc_content)
		// {
		// 	// Implement heredoc
		// }
		// if (!ft_strncmp(current_command->argv[0], "cd", 3))
		// {
		// 	ft_cd(current_command->argv[1], program);
		// 	current_command = current_command->next;
		// 	// change directory in pwd by using __environ
		// 	continue;
		// }
		// else if (!ft_strncmp(current_command->argv[0], "unset", 6))
		// {
		// 	// Implement unset
		// }
		if (!ft_strncmp(current_command->argv[0], "exit", 5))
		{
			bool is_digit;
			int i;

			ft_putstr_fd("exit\n", 1);
			if (ft_array_len(current_command->argv) == 1)
				free_program((*program), 0);
			for (int i = 0; current_command->argv[1][i]; i++)
			{
				if (!isdigit(current_command->argv[1][i]))
				{
					is_digit = false;
					break;
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
				ft_putstrs_fd("bash: exit: ", current_command->argv[1], ": numeric argument required\n", 2);
				free_program((*program), 2);
			}
			else if (current_command->argv[1] && is_digit)
				free_program((*program), i % 256);
			else
			{
				if (!is_digit)
				{
					ft_putstrs_fd("bash: exit: ", current_command->argv[1], ": numeric argument required\n", 2);
					free_program((*program), 2);
				}
				free_program((*program), 0);
			}
		}
	else
	{
		pid_t pid = fork();
		if (pid == 0)
		{
			// This is the child process
			if (current_command->next)
				dup2(fd[1], 1);
			if (fd[0] != 0)
				dup2(fd[0], 0);
			if (in != 0)
				dup2(in, 0);
			if (out != 1)
				dup2(out, 1);
			if (!ft_strncmp(current_command->argv[0], "env", 4))
			{
				for (int i = 0; (*program)->envp[i]; i++)
				{
					if (ft_strncmp((*program)->envp[i], "_=", 2))
						printf("%s\n", (*program)->envp[i]);
				}
			}
			else if (!ft_strncmp(current_command->argv[0], "pwd", 4))
				printf("%s\n", getcwd(NULL, 0));
			else if (!ft_strncmp(current_command->argv[0], "echo", 5))
			{
				if (current_command->argv[1] && !ft_strncmp(current_command->argv[1], "-n", 3))
				{
					for (int i = 2; current_command->argv[i]; i++)
					{
						printf("%s", current_command->argv[i]);
						if (current_command->argv[i + 1])
							printf(" ");
					}
				}
				else
				{
					for (int i = 1; current_command->argv[i]; i++)
					{
						printf("%s", current_command->argv[i]);
						if (current_command->argv[i + 1])
							printf(" ");
					}
					printf("\n");
				}
			}
			else
			{
				if (current_command->argv[0][0] == '/')
					cmd_path = ft_strdup(current_command->argv[0]);
				else
					cmd_path = find_path((*program)->envp, current_command->argv[0]);
				int execstat = execve(cmd_path, current_command->argv, (*program)->envp);
				if (execstat == -1)
				{
					printf("minishell: %s: command not found\n", current_command->argv[0]);
					free(cmd_path);
					exit(1);
				}
				free(cmd_path);
			}
			exit (0);
		}
		else if (pid > 0)
		{

			// This is the parent process
			if (!ft_strncmp(current_command->argv[0], "export", 7))
			{
				ft_export(program, current_command->argv);
			}
			// Wait for the child process to finish

			int status;
			waitpid(pid, &status, 0);
			
			
			if (WIFEXITED(status))
			{
				printf("Program exited with status %d\n", WEXITSTATUS(status));
				ft_export(program, (char *[]){"export", ft_strjoin("?=", ft_itoa(WEXITSTATUS(status))), NULL});
			}
			if (WIFSIGNALED(status))
			{
				printf("Program was killed by signal %d\n", WTERMSIG(status));
				printf("Program exited with status %d\n", status);
				ft_export(program, (char *[]){"export", ft_strjoin("?=", ft_itoa(status + 128)), NULL});
			}
		}
		else
		{
			// Fork failed
			printf("fork failed\n");
		}
		current_command = current_command->next;
	}
}
}
