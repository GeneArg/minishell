/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperez-a <bperez-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 14:29:43 by eagranat          #+#    #+#             */
/*   Updated: 2024/05/27 13:51:46 by bperez-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	**sort_env(t_program *program)
{
	char	**sorted_envp;
	int		i;
	int		j;
	int		len;

	i = 0;
	j = 0;
	sorted_envp = ft_copy_array(program->envp);
	len = ft_array_len(sorted_envp);
	while (i++ < len)
	{
		j = 0;
		while (j < len - 1)
		{
			if (ft_strncmp(sorted_envp[j], sorted_envp[j + 1],
					ft_strlen(sorted_envp[j + 1])) > 0)
				ft_swap(&sorted_envp[j], &sorted_envp[j + 1], sizeof(char *));
			j++;
		}
	}
	return (sorted_envp);
}

void	add_env(char ***envp, char *new_env)
{
	int	i;
	int	new_env_len;
	int	new_env_key_len;

	i = 0;
	new_env_len = ft_strlen(new_env);
	new_env_key_len = ft_strchr(new_env, '=') - new_env;
	while ((*envp)[i] != NULL && ft_strncmp((*envp)[i], new_env,
			new_env_key_len))
		i++;
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
		ft_append_str_array(envp, new_env);
	}
}

int	check_if_valid_name(char *name)
{
	int	i;

	i = 0;
	if (!ft_isalpha(name[0]) && name[0] != '_' && ft_strncmp(name, "?=", 2))
	{
		ft_error(NULL, "export", "not a valid identifier", -1);
		return (FAILURE);
	}
	while (name[++i])
	{
		if (name[i] == '=')
			break ;
		if (!ft_isalnum(name[i]) && name[i] != '_' && ft_strncmp(name, "?=", 2))
		{
			ft_error(NULL, "export", "not a valid identifier", -1);
			return (FAILURE);
		}
	}
	return (SUCCESS);
}

void	put_env(char **envp)
{
	int		i;
	char	*env;
	int		key_len;

	i = 0;
	while (envp[i])
	{
		env = envp[i];
		key_len = 0;
		while (env[key_len] && env[key_len] != '=')
			key_len++;
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

int	ft_export(t_program **program, char **argv)
{
	char	**args;
	int		i;
	char	**sorted_envp;

	i = 0;
	args = argv;
	if (!args[1])
	{
		sorted_envp = sort_env(*program);
		put_env(sorted_envp);
		ft_free_array(sorted_envp);
	}
	else
	{
		i = 1;
		while (args[i])
		{
			if (check_if_valid_name(args[i]))
				return (FAILURE);
			add_env(&((*program)->envp), args[i]);
			i++;
		}
	}
	return (SUCCESS);
}
