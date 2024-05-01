/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperez-a <bperez-a@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 14:35:37 by bperez-a          #+#    #+#             */
/*   Updated: 2024/05/01 10:09:56 by bperez-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command	*init_command(void)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->argv = NULL;
	cmd->redirect_in = NULL;
	cmd->redirect_out = NULL;
	cmd->append = 0;
	cmd->heredoc_content = NULL;
	cmd->next = NULL;
	return (cmd);
}

void	append_argument(t_command **cmd, char *arg)
{
	int	argc;

	argc = 0;
	if ((*cmd)->argv)
	{
		while ((*cmd)->argv[argc])
			argc++;
	}
	(*cmd)->argv = realloc((*cmd)->argv, sizeof(char *) * (argc + 2));
	(*cmd)->argv[argc] = strdup(arg);
	(*cmd)->argv[argc + 1] = NULL;
}

t_command	*parse(t_token *token)
{
	t_command	*head;
	t_command	*current_cmd;
	t_command	*new_cmd;

	head = NULL;
	current_cmd = NULL;
	while (token)
	{
		if (!head || token->type == TOKEN_PIPE)
		{
			new_cmd = init_command();
			if (!head)
			{
				head = new_cmd;
				current_cmd = head;
			}
			else
			{
				current_cmd->next = new_cmd;
				current_cmd = new_cmd;
			}
		}
		if (token->type == TOKEN_WORD)
			append_argument(&current_cmd, token->value);
		if (token->type == TOKEN_REDIRECT_IN)
		{
			token = token->next;
			if (token)
				current_cmd->redirect_in = ft_strdup(token->value);
		}
		if (token->type == TOKEN_REDIRECT_OUT)
		{
			token = token->next;
			if (token)
				current_cmd->redirect_out = ft_strdup(token->value);
		}
		if (token->type == TOKEN_REDIRECT_APPEND)
		{
			token = token->next;
			if (token)
			{
				current_cmd->redirect_out = ft_strdup(token->value);
				current_cmd->append = 1;
			}
		}
		if (token->type == TOKEN_HEREDOC)
		{
			token = token->next;
			if (token)
				current_cmd->redirect_in = ft_strdup(token->value);
		}
		token = token->next;
	}
	return (head);
}
