/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_split.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperez-a <bperez-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 12:07:05 by bperez-a          #+#    #+#             */
/*   Updated: 2024/05/16 10:57:06 by bperez-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	**splitter(char *input)
{
	bool	in_double_quotes;
	bool	in_simple_quotes;
	int		i;
	int		j;
	char	**ret;
	char	*current_word;

	ret = malloc(sizeof(char *) * 1000);
	if (!ret)
		return (NULL);
	current_word = malloc(sizeof(char) * 1000);
	if (!current_word)
	{
		free(ret);
		return (NULL);
	}
	current_word[0] = '\0';
	i = 0;
	j = 0;
	in_double_quotes = false;
	in_simple_quotes = false;
	while (*input)
	{
		if (*input == '#' && !in_double_quotes && !in_simple_quotes)
			break;
		if (*input == '"' && !in_simple_quotes)
		{
			in_double_quotes = !in_double_quotes;
			current_word[j++] = *input;
		}
		else if (*input == '\'' && !in_double_quotes)
		{
			in_simple_quotes = !in_simple_quotes;
			current_word[j++] = *input;
		}
		else if (*input == ' ' && !in_double_quotes && !in_simple_quotes)
		{
			if (j != 0)
			{
				current_word[j] = '\0';
				ret[i++] = ft_strdup(current_word);
				j = 0;
			}
		}
		else if (ft_strchr("|><", *input) && !in_double_quotes
			&& !in_simple_quotes)
		{
			if (j != 0)
			{
				current_word[j] = '\0';
				ret[i++] = ft_strdup(current_word);
				j = 0;
			}
			if ((*input == '>' || *input == '<') && *(input + 1) == *input)
			{
				ret[i++] = ft_substr(input, 0, 2);
				input++;
			}
			else
				ret[i++] = ft_substr(input, 0, 1);
		}
		else
			current_word[j++] = *input;
		input++;
	}
	if (j != 0)
	{
		current_word[j] = '\0';
		ret[i++] = ft_strdup(current_word);
	}
	free(current_word);
	ret[i] = NULL;
	return (ret);
}
