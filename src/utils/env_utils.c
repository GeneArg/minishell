/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eagranat <eagranat@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 11:11:00 by bperez-a          #+#    #+#             */
/*   Updated: 2024/05/25 22:45:21 by eagranat         ###   ########.fr       */
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

char *find_path(char **envp, char *cmd)
{
    char **paths;
    char *cmd_path;
    char *sub;
    char **temp_paths;

    paths = get_paths(envp);
    temp_paths = paths;
    sub = ft_strjoin("/", cmd);
    while (*temp_paths)
    {
        cmd_path = ft_strjoin(*temp_paths, sub);
        // printf("Trying path: %s\n", cmd_path); // Debug statement
        if (!access(cmd_path, F_OK))
            break;
        free(cmd_path);
        cmd_path = NULL;
        temp_paths++;
    }
    free(sub);
    ft_free_array(paths);
    return cmd_path;
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


void set_env_var(t_program **program, char *key, char *value)
{
	int	i;
	char	*new_env_var;

	i = find_env_var((*program)->envp, key);
	if (i == -1)
		add_env(&(*program)->envp, ft_strjoin(key, ft_strjoin("=", value)));
	else
	{
		new_env_var = ft_strjoin(key, ft_strjoin("=", value));
		free((*program)->envp[i]);
		(*program)->envp[i] = new_env_var;
	}
}

char **ft_dup_array(char **array)
{
    int i = 0;
    char **new_array;

    if (!array)
        return NULL;

    while (array[i])
        i++;

    new_array = (char **)malloc(sizeof(char *) * (i + 1));
    if (!new_array)
        return NULL;

    i = 0;
    while (array[i])
    {
        new_array[i] = ft_strdup(array[i]);
        if (!new_array[i])
        {
            while (i > 0)
            {
                free(new_array[--i]);
            }
            free(new_array);
            return NULL;
        }
        i++;
    }
    new_array[i] = NULL;
    return new_array;
}

void ft_free_array(char **array)
{
    int i = 0;

    if (!array)
        return;

    while (array[i])
    {
        free(array[i]);
        i++;
    }
    free(array);
}
