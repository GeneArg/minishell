/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperez-a <bperez-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 14:35:37 by bperez-a          #+#    #+#             */
/*   Updated: 2024/05/29 12:03:17 by bperez-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	initialize_new_command(t_command **head, t_command **current_cmd)
{
	t_command	*new_cmd;

	new_cmd = malloc(sizeof(t_command));
	new_cmd->argv = NULL;
	new_cmd->redirects = NULL;
	new_cmd->append = 0;
	new_cmd->next = NULL;
	new_cmd->flag_error = 0;
	if (!*head)
	{
		*head = new_cmd;
		*current_cmd = *head;
	}
	else
	{
		(*current_cmd)->next = new_cmd;
		*current_cmd = new_cmd;
	}
}

t_command	*parse(t_token *token)
{
	t_command	*head;
	t_command	*current_cmd;

	head = NULL;
	current_cmd = NULL;
	while (token)
	{
		if (!head || token->type == TOKEN_PIPE)
			initialize_new_command(&head, &current_cmd);
		if (token->type == TOKEN_WORD)
			append_argument(&current_cmd, token->value);
		if (token->type == TOKEN_REDIRECT_IN)
			handle_token_redirect_in(&token, &current_cmd);
		if (token->type == TOKEN_REDIRECT_OUT
			|| token->type == TOKEN_REDIRECT_APPEND)
			handle_token_redirect_out(&token, &current_cmd);
		token = token->next;
	}
	return (head);
}
