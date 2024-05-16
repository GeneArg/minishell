/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperez-a <bperez-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 15:44:02 by bperez-a          #+#    #+#             */
/*   Updated: 2024/05/16 12:57:14 by bperez-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


void	display_lexer_output(t_token *tokens)
{
	t_token	*tmp;

	tmp = tokens;
	while (tmp)
	{
		printf("type: %d, value: %s\n", tmp->type, tmp->value);
		tmp = tmp->next;
	}
}

void	display_args(t_command *commands)
{
	t_command	*tmp;
	int			i;

	tmp = commands;
	while (tmp)
	{
		i = 0;
		while (tmp->argv[i])
		{
			printf("args[%d]: %s\n", i, tmp->argv[i]);
			i++;
		}
		if (tmp->redirect_in)
		{
			printf("redirect_in:\n");
			t_redirection *redir = tmp->redirect_in;
			while (redir)
			{
				printf("file: %s\n", redir->file);
				redir = redir->next;
			}
		}
		if (tmp->redirect_out)
		{
			printf("redirect_out:\n");
			t_redirection *redir = tmp->redirect_out;
			while (redir)
			{
				printf("file: %s\n", redir->file);
				redir = redir->next;
			}
		}
		if (tmp->heredoc_content)
			printf("heredoc_content: %s\n", tmp->heredoc_content);
		
		tmp = tmp->next;
	}
}