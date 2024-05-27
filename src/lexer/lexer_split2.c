/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_split2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperez-a <bperez-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 16:44:25 by bperez-a          #+#    #+#             */
/*   Updated: 2024/05/27 16:44:34 by bperez-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	add_current_word_to_ret(char **ret, t_split *split_vars)
{
	if (split_vars->j != 0)
	{
		split_vars->current_word[split_vars->j] = '\0';
		ret[split_vars->i++] = strdup(split_vars->current_word);
		split_vars->j = 0;
	}
}

void	handle_quotes(char **input, t_split *split_vars, char quote)
{
	if (quote == '"')
		split_vars->in_double_quotes = !split_vars->in_double_quotes;
	else if (quote == '\'')
		split_vars->in_simple_quotes = !split_vars->in_simple_quotes;
	split_vars->current_word[split_vars->j++] = **input;
}

void	handle_special_chars(char **input, char **ret, t_split *split_vars)
{
	add_current_word_to_ret(ret, split_vars);
	if ((**input == '>' || **input == '<') && *(*input + 1) == **input)
	{
		ret[split_vars->i++] = strndup(*input, 2);
		(*input)++;
	}
	else
		ret[split_vars->i++] = strndup(*input, 1);
}

void	handle_space(char **ret, t_split *split_vars)
{
	if (split_vars->j != 0)
	{
		split_vars->current_word[split_vars->j] = '\0';
		ret[split_vars->i++] = strdup(split_vars->current_word);
		split_vars->j = 0;
	}
}

void	finalize_current_word(char **ret, t_split *split_vars)
{
	if (split_vars->j != 0)
	{
		split_vars->current_word[split_vars->j] = '\0';
		ret[split_vars->i++] = strdup(split_vars->current_word);
	}
	ret[split_vars->i] = NULL;
}
