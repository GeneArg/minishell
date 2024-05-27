/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperez-a <bperez-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 11:24:35 by bperez-a          #+#    #+#             */
/*   Updated: 2024/05/27 12:02:35 by bperez-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_token_type	determine_type(char *token)
{
	if (token[0] == '|')
		return (TOKEN_PIPE);
	else if (token[0] == '<')
		return (TOKEN_REDIRECT_IN);
	else if (token[0] == '>')
	{
		if (token[1] == '>')
			return (TOKEN_REDIRECT_APPEND);
		return (TOKEN_REDIRECT_OUT);
	}
	return (TOKEN_WORD);
}

void	append_token(t_token **current, t_token_type type, char *value)
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
			head->value = ft_strdup(split[i]);
			head->next = NULL;
			current = head;
		}
		else
			append_token(&current, determine_type(split[i]),
				ft_strdup(split[i]));
		i++;
	}
	return (head);
}

t_token	*lex(char *input)
{
	char	**split;
	t_token	*token;

	split = splitter(input);
	if (!split)
		return (NULL);
	token = populate_list(split);
	if (!token)
		return (NULL);
	ft_free_array(split);
	return (token);
}
