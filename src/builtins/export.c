/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eagranat <eagranat@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 14:29:43 by eagranat          #+#    #+#             */
/*   Updated: 2024/05/04 17:16:16 by eagranat         ###   ########.fr       */
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

void add_env(char ***envp, char *new_env) {
    int i = 0;
	printf("we are here 1");
    while ((*envp)[i] != NULL && ft_strncmp((*envp)[i], new_env, ft_strlen(new_env) - (ft_strchr(new_env, '=') - new_env)))
    {   
	    i++;
	}
	if ((*envp)[i] != NULL && ft_strchr((*envp)[i], '='))
	{
		printf("we are here 2");
		// free(envp[i]);
		(*envp)[i] = ft_strdup(new_env);
		//return envp;
	}
	else
	{
		printf("we are here 3");
		append_str_to_array(envp, new_env);
		//return envp;
	}
		
	
    // char **new_envp = malloc((i + 2) * sizeof(char *)); // +2 because one for new env and one for NULL

    // // Copy the old environment variables to the new array
    // for (int j = 0; j < i; j++)
    //     new_envp[j] = envp[j];
    // // Add the new environment variable
    // new_envp[i] = new_env;
    // // Set the last element to NULL
    // new_envp[i + 1] = NULL;
    // // Free the old environment variables array
    // free(envp);

    // return new_envp;
}

void	ft_export(t_program **program, t_command *command)
{
	char **args;
	int i = 0;

	args = command->argv;
	if (!args[1])
	{
		char **sorted_envp = sort_env(*program);
		while (sorted_envp[i])
		{
			if (ft_strncmp(sorted_envp[i], "_=", 2) == 0)
			{
				i++;
				continue;
			}
			ft_putstr_fd("declare -x ", 1);
			while (*sorted_envp[i] != '=')
				ft_putchar_fd(*sorted_envp[i]++, 1);
			if (*sorted_envp[i] == '=')
			{
				ft_putchar_fd('=', 1);
				ft_putchar_fd('\"', 1);
				sorted_envp[i]++;
				while (*sorted_envp[i])
					ft_putchar_fd(*sorted_envp[i]++, 1);
				ft_putchar_fd('\"', 1);
			}
			ft_putchar_fd('\n', 1);
			i++;
		}
		return ;
	}
	else
	{
		i = 1;
		while (args[i])
		{
			add_env(&((*program)->envp), args[i]);
			i++;
		}
	}
}