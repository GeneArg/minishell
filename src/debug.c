/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperez-a <bperez-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 15:44:02 by bperez-a          #+#    #+#             */
/*   Updated: 2024/05/17 09:59:26 by bperez-a         ###   ########.fr       */
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
		if (tmp->redirects)
		{
			printf("redirect:\n");
			t_redirection *redir = tmp->redirects;
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
