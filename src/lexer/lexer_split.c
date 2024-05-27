/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_split.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperez-a <bperez-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 12:07:05 by bperez-a          #+#    #+#             */
/*   Updated: 2024/05/27 16:44:47 by bperez-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_split	*initialize_split_vars(void)
{
	t_split	*split_vars;

	split_vars = malloc(sizeof(t_split));
	if (!split_vars)
		return (NULL);
	split_vars->current_word = malloc(sizeof(char) * 1000);
	if (!split_vars->current_word)
	{
		free(split_vars);
		return (NULL);
	}
	split_vars->current_word[0] = '\0';
	split_vars->i = 0;
	split_vars->j = 0;
	split_vars->in_double_quotes = false;
	split_vars->in_simple_quotes = false;
	return (split_vars);
}

void	process_input_string(char *input, char **ret, t_split *split_vars)
{
	while (*input)
	{
		if (*input == '#' && !split_vars->in_double_quotes
			&& !split_vars->in_simple_quotes)
			break ;
		if (*input == '"' && !split_vars->in_simple_quotes)
			handle_quotes(&input, split_vars, '"');
		else if (*input == '\'' && !split_vars->in_double_quotes)
			handle_quotes(&input, split_vars, '\'');
		else if (*input == ' ' && !split_vars->in_double_quotes
			&& !split_vars->in_simple_quotes)
			handle_space(ret, split_vars);
		else if (strchr("|><", *input) && !split_vars->in_double_quotes
			&& !split_vars->in_simple_quotes)
			handle_special_chars(&input, ret, split_vars);
		else
			split_vars->current_word[split_vars->j++] = *input;
		input++;
	}
}

char	**splitter(char *input)
{
	char	**ret;
	t_split	*split_vars;

	ret = malloc(sizeof(char *) * 1000);
	if (!ret)
		return (NULL);
	split_vars = initialize_split_vars();
	if (!split_vars)
	{
		free(ret);
		return (NULL);
	}
	process_input_string(input, ret, split_vars);
	finalize_current_word(ret, split_vars);
	free(split_vars->current_word);
	free(split_vars);
	return (ret);
}
