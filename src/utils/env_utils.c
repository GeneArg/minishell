/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperez-a <bperez-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 11:11:00 by bperez-a          #+#    #+#             */
/*   Updated: 2024/06/04 13:44:37 by bperez-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	**get_paths(char **envp)
{
	char	*sub;
	char	**paths;
	char	*pwd;
	int		i;

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
	ft_append_str_array(&paths, pwd);
	free(pwd);
	return (paths);
}

char	*find_path(char **envp, char *cmd)
{
	char	**paths;
	char	*cmd_path;
	char	*sub;
	char	**temp_paths;

	if (cmd[0] == '.' || cmd[0] == '/')
	{
		cmd_path = ft_strdup(cmd);
		return (cmd_path);
	}
	paths = get_paths(envp);
	temp_paths = paths;
	sub = ft_strjoin("/", cmd);
	while (*temp_paths)
	{
		cmd_path = ft_strjoin(*temp_paths, sub);
		if (!access(cmd_path, F_OK))
			break ;
		free(cmd_path);
		cmd_path = NULL;
		temp_paths++;
	}
	free(sub);
	ft_free_array(paths);
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

void	set_env_var(t_program **program, char *key, char *value)
{
	int		i;
	char	*new_env_var;
	char	*temp;

	i = find_env_var((*program)->envp, key);
	temp = ft_strjoin(key, "=");
	new_env_var = ft_strjoin(temp, value);
	free(temp);
	if (i == -1)
	{
		add_env(&(*program)->envp, new_env_var);
	}
	else
	{
		free((*program)->envp[i]);
		(*program)->envp[i] = new_env_var;
	}
}
