/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperez-a <bperez-a@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 11:24:35 by bperez-a          #+#    #+#             */
/*   Updated: 2024/05/01 08:50:22 by bperez-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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

TokenType	determine_type(char *token)
{
	if (token[0] == '|')
		return (TOKEN_PIPE);
	else if (token[0] == '<')
	{
		if (token[1] == '<')
			return (TOKEN_HEREDOC);
		return (TOKEN_REDIRECT_IN);
	}
	else if (token[0] == '>')
	{
		if (token[1] == '>')
			return (TOKEN_REDIRECT_APPEND);
		return (TOKEN_REDIRECT_OUT);
	}
	return (TOKEN_WORD);
}

void	append_token(t_token **current, TokenType type, char *value)
{
	t_token	*new_node;

	new_node = malloc(sizeof(t_token));
	if (new_node == NULL)
		return ;
	new_node->type = type;
	new_node->value = value;
	new_node->next = NULL;
	(*current)->next = new_node;
	*current = new_node;
}

t_token	*populate_list(char **split)
{
	t_token	*head;
	t_token	*current;
	int		i;

	head = NULL;
	current = NULL;
	i = 0;
	while (split[i])
	{
		if (!head)
		{
			head = malloc(sizeof(t_token));
			if (!head)
				return (NULL);
			head->type = determine_type(split[i]);
			head->value = split[i];
			head->next = NULL;
			current = head;
		}
		else
			append_token(&current, determine_type(split[i]), split[i]);
		i++;
	}
	return (head);
}

t_token	*lex(char *input)
{
	char	**split;
	t_token	*token;

	split = splitter(input);
	token = populate_list(split);
	return (token);
}
