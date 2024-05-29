/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   append.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperez-a <bperez-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 12:03:23 by bperez-a          #+#    #+#             */
/*   Updated: 2024/05/29 12:04:32 by bperez-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	**create_and_copy_new_argv(t_command **cmd, char *new_arg)
{
	int		current_arg_count;
	char	**updated_argv;
	int		i;

	current_arg_count = 0;
	if ((*cmd)->argv)
	{
		while ((*cmd)->argv[current_arg_count])
			current_arg_count++;
	}
	updated_argv = (char **)malloc(sizeof(char *) * (current_arg_count + 2));
	i = 0;
	while (i < current_arg_count)
	{
		updated_argv[i] = (*cmd)->argv[i];
		i++;
	}
	updated_argv[current_arg_count] = ft_strdup(new_arg);
	updated_argv[current_arg_count + 1] = NULL;
	return (updated_argv);
}

void	handle_quote(const char *arg, char *new_arg, int *i, int *j)
{
	char	quote;

	quote = arg[*i];
	new_arg[(*j)++] = arg[(*i)++];
	while (arg[*i] != '\0' && arg[*i] != quote)
	{
		new_arg[(*j)++] = arg[(*i)++];
	}
	if (arg[*i] == quote)
	{
		new_arg[(*j)++] = arg[(*i)++];
	}
}

char	*copy_argument_with_quotes(const char *arg)
{
	int		i;
	int		j;
	char	*new_arg;

	i = 0;
	j = 0;
	new_arg = (char *)malloc(strlen(arg) + 1);
	if (new_arg == NULL)
	{
		ft_putstr_fd("Memory allocation failed\n", 2);
		return (NULL);
	}
	while (arg[i] != '\0')
	{
		if (arg[i] == '\'' || arg[i] == '\"')
		{
			handle_quote(arg, new_arg, &i, &j);
		}
		else
		{
			new_arg[j++] = arg[i++];
		}
	}
	new_arg[j] = '\0';
	return (new_arg);
}

void	append_argument(t_command **cmd, char *arg)
{
	char	*new_arg;
	char	**updated_argv;

	new_arg = copy_argument_with_quotes(arg);
	if (new_arg == NULL)
		return ;
	updated_argv = create_and_copy_new_argv(cmd, new_arg);
	if ((*cmd)->argv)
		free((*cmd)->argv);
	(*cmd)->argv = updated_argv;
	free(new_arg);
}
