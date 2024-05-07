/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eagranat <eagranat@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 14:29:43 by eagranat          #+#    #+#             */
/*   Updated: 2024/05/07 23:11:57 by eagranat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"



char **sort_env(t_program *program)
{
	char **sorted_envp;
	int i = 0;
	int j = 0;
	int len = 0;

	while (program->envp[len])
		len++;
	sorted_envp = (char **)malloc(sizeof(char *) * (len + 1));
	while (i < len)
	{
		sorted_envp[i] = program->envp[i];
		i++;
	}
	sorted_envp[i] = NULL;
	i = 0;
	while (i < len)
	{
		j = 0;
		while (j < len - 1)
		{
			if (ft_strncmp(sorted_envp[j], sorted_envp[j + 1], ft_strlen(sorted_envp[j + 1])) > 0)
			{
				char *tmp = sorted_envp[j];
				sorted_envp[j] = sorted_envp[j + 1];
				sorted_envp[j + 1] = tmp;
			}
			j++;
		}
		i++;
	}
	return (sorted_envp);
}

void add_env(char ***envp, char *new_env)
{
    int i = 0;
    int new_env_len = ft_strlen(new_env);
    int new_env_key_len = ft_strchr(new_env, '=') - new_env;

    // Find the existing variable or the end of the array
    while ((*envp)[i] != NULL && ft_strncmp((*envp)[i], new_env, new_env_key_len))
    {
        i++;
    }

    // If variable exists, update it
    if ((*envp)[i] != NULL && ft_strchr((*envp)[i], '='))
    {
        free((*envp)[i]);
        (*envp)[i] = (char *)malloc((new_env_len + 1) * sizeof(char));
        if ((*envp)[i] != NULL)
        {
            ft_strlcpy((*envp)[i], new_env, new_env_len + 1);
        }
    }
    else
    {
        append_str_to_array(envp, new_env);
    }
}

int	check_if_valid_name(char *name)
{
	int i = 0;

	if (!ft_isalpha(name[0]) && name[0] != '_' && ft_strncmp(name, "?=", 2))
	{
		ft_putstr_fd("bash: export: `", 2);
		ft_putstr_fd(name, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		return (1);
	}
	while (name[++i])
	{
		if (!ft_isalnum(name[i]) && name[i] != '_' && name[i] != '/' && ft_strncmp(name, "?=", 2) && name[i] != '=')
		{
			ft_putstr_fd("bash: export: `", 2);
			ft_putstr_fd(name, 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			return (1);
		}
	}
	return (0);
}

int ft_export(t_program **program, char **argv)
{
    char **args;
    int i = 0;

    args = argv;
    if (!args[1])
    {
        char **sorted_envp = sort_env(*program);
        while (sorted_envp[i])
        {
            char *env = sorted_envp[i];
            int key_len = 0;
            while (env[key_len] && env[key_len] != '=')
            {
                key_len++;
            }

            ft_putstr_fd("declare -x ", 1);
            write(1, env, key_len);

            if (env[key_len] == '=')
            {
                ft_putstr_fd("=\"", 1);
                write(1, env + key_len + 1, ft_strlen(env) - key_len - 1);
                ft_putstr_fd("\"\n", 1);
            }
            else
            {
                ft_putchar_fd('\n', 1);
            }
            i++;
        }
    }
    else
    {
		if (check_if_valid_name(args[1]))
			return (1);
        for (i = 1; args[i]; i++)
        {
            add_env(&((*program)->envp), args[i]);
        }
    }
	return (0);
}


