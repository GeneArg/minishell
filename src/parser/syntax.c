/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperez-a <bperez-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 15:29:48 by bperez-a          #+#    #+#             */
/*   Updated: 2024/05/17 09:58:41 by bperez-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	check_syntax(t_token *token, t_program **program)
{
	t_token	*current;
	char	*error_msg;

	current = token;
	if (!current)
		return (1);
	if (current->type == TOKEN_PIPE)
	{
		ft_error(program, NULL, "syntax error near unexpected token `|'", 2);
		return (1);
	}
	while (current)
	{
		if (current->type == TOKEN_PIPE && current->next
			&& (current->next->type == TOKEN_REDIRECT_IN
				|| current->next->type == TOKEN_REDIRECT_OUT))
		{
			current = current->next;
			continue ;
		}
		if (current->type != TOKEN_WORD && current->next == NULL)
		{
			ft_error(program, NULL,
				"syntax error near unexpected token `newline'", 2);
			return (1);
		}
		if (current->type != TOKEN_WORD && current->next->type != TOKEN_WORD)
		{
			error_msg = ft_strjoin("syntax error near unexpected token `",
					current->next->value);
			error_msg = ft_strjoin(error_msg, "'");
			ft_error(program, NULL, error_msg, 2);
			free(error_msg);
			return (1);
		}
		current = current->next;
	}
	return (0);
}
