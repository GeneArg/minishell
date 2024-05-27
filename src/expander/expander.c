/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperez-a <bperez-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 12:56:22 by bperez-a          #+#    #+#             */
/*   Updated: 2024/05/27 12:15:30 by bperez-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

bool	is_enclosed_in_single_quotes(char *arg)
{
	int	len;

	len = ft_strlen(arg);
	return (len >= 2 && arg[0] == '\'' && arg[len - 1] == '\'');
}

void	trim_quotes_helper(char *input, char *output)
{
	int		i;
	int		j;
	bool	inside_quotes;
	char	current_quote;

	i = 0;
	j = 0;
	inside_quotes = false;
	current_quote = 0;
	while (input[i] != '\0')
	{
		if ((input[i] == '\'' || input[i] == '\"') && (!inside_quotes || \
			current_quote == input[i]))
		{
			if (!inside_quotes)
			{
				inside_quotes = true;
				current_quote = input[i];
			}
			else
			{
				inside_quotes = false;
				current_quote = 0;
			}
		}
		else
			output[j++] = input[i];
		i++;
	}
	output[j] = '\0';
}

void	trim_quotes(char **arg)
{
	char	*input;
	char	*output;

	input = *arg;
	output = malloc(ft_strlen(input) + 1);
	if (!output)
	{
		perror("Memory allocation failed");
		return ;
	}
	trim_quotes_helper(input, output);
	free(*arg);
	*arg = output;
}

void	replace_env_helper(char **cur, char **out, char **env)
{
	char	var_name[256];
	char	*value;
	char	*end;
	int		i;

	end = *cur + 1;
	if (*end != '?')
		while (ft_isalnum((unsigned char)*end) || *end == '_' || *end == '?')
			end++;
	else
		end++;
	ft_strlcpy(var_name, *cur + 1, end - (*cur + 1) + 1);
	value = NULL;
	i = 0;
	while (env[i] != NULL)
	{
		if (ft_strncmp(env[i], var_name, end - (*cur + 1)) == 0 && \
			env[i][end - (*cur + 1)] == '=')
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
		if (*cur == '$' && (*(cur + 1) == '?' || ft_isalnum((unsigned char) \
			*(cur + 1)) || *(cur + 1) == '_'))
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

void	expand_args_helper(char **arg, char **envp)
{
	if (is_enclosed_in_single_quotes(*arg))
		trim_quotes(arg);
	else
	{
		trim_quotes(arg);
		replace_env_variables(arg, envp);
	}
}

void	expand_args(t_command *cmd, char **envp)
{
	int	i;

	i = 0;
	while (cmd->argv[i] != NULL)
	{
		expand_args_helper(&(cmd->argv[i]), envp);
		i++;
	}
	remove_empty_args(&(cmd->argv));
}

void	expand_redirects_helper(t_redirection *redir, char **envp)
{
	if (is_enclosed_in_single_quotes(redir->file))
		trim_quotes(&(redir->file));
	else
	{
		trim_quotes(&(redir->file));
		replace_env_variables(&(redir->file), envp);
	}
}

void	expand_redirects(t_redirection *redirects, char **envp)
{
	while (redirects != NULL)
	{
		expand_redirects_helper(redirects, envp);
		redirects = redirects->next;
	}
}

void	expand(t_command *commands, char **envp)
{
	t_command	*current;

	current = commands;
	while (current != NULL)
	{
		expand_args(current, envp);
		expand_redirects(current->redirects, envp);
		current = current->next;
	}
}
