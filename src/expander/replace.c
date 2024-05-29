/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperez-a <bperez-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 12:40:46 by bperez-a          #+#    #+#             */
/*   Updated: 2024/05/29 12:44:16 by bperez-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*find_var_end(char *cur)
{
	char	*end;

	end = cur + 1;
	if (*end != '?')
	{
		while (ft_isalnum((unsigned char)*end) || *end == '_' || *end == '?')
			end++;
	}
	else
	{
		end++;
	}
	return (end);
}

void	replace_env_helper(char **cur, char **out, char **env)
{
	char	var_name[256];
	char	*value;
	char	*end;
	int		i;

	end = find_var_end(*cur);
	ft_strlcpy(var_name, *cur + 1, end - (*cur + 1) + 1);
	value = NULL;
	i = 0;
	while (env[i] != NULL)
	{
		if (ft_strncmp(env[i], var_name, end - (*cur + 1)) == 0 && env[i][end
			- (*cur + 1)] == '=')
		{
			value = env[i] + (end - (*cur + 1)) + 1;
			break ;
		}
		i++;
	}
	if (value)
	{
		ft_strlcpy(*out, value, ft_strlen(value) + 1);
		*out += ft_strlen(value);
	}
	*cur = end;
}

void	replace_env_variables(char **arg, char **env)
{
	char	*input;
	char	*output;
	char	*cur;
	char	*out;

	input = *arg;
	output = malloc(ft_strlen(input) + 1024);
	if (!output)
	{
		ft_putstr_fd("Memory allocation failed", 2);
		return ;
	}
	cur = input;
	out = output;
	while (*cur)
	{
		if (*cur == '$' && (*(cur + 1) == '?' || ft_isalnum((unsigned char)*(cur
						+ 1)) || *(cur + 1) == '_'))
			replace_env_helper(&cur, &out, env);
		else
			*out++ = *cur++;
	}
	*out = '\0';
	free(*arg);
	*arg = output;
}

void	remove_empty_args_helper(char **argv, char **new_argv, int *count)
{
	int	i;

	i = 0;
	while (argv[i] != NULL)
	{
		if (ft_strlen(argv[i]) > 0)
		{
			new_argv[*count] = argv[i];
			(*count)++;
		}
		i++;
	}
	new_argv[*count] = NULL;
}

void	remove_empty_args(char ***argv)
{
	char	**new_argv;
	int		count;

	new_argv = malloc(sizeof(char *) * (ft_array_len(*argv) + 1));
	if (!new_argv)
		return ;
	count = 0;
	remove_empty_args_helper(*argv, new_argv, &count);
	free(*argv);
	*argv = new_argv;
}
