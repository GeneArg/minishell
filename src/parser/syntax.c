/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperez-a <bperez-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 15:29:48 by bperez-a          #+#    #+#             */
/*   Updated: 2024/05/13 15:29:56 by bperez-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	check_syntax(t_token *token, t_program **program)
{
	t_token	*current;

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
		if (current->type == TOKEN_PIPE
			&& current->next && (current->next->type == TOKEN_REDIRECT_IN
				|| current->next->type == TOKEN_REDIRECT_OUT))
		{
			current = current->next;
			continue ;
		}
		if (current->type != TOKEN_WORD && current->next == NULL)
		{
			ft_error(program, NULL, "syntax error near unexpected token `newline'", 2);
			return (1);
		}
		if (current->type != TOKEN_WORD && current->next->type != TOKEN_WORD)
		{
			ft_putstr_fd("bash: syntax error near unexpected token `", 2);
			ft_putstr_fd(current->next->value, 2);
			ft_putstr_fd("'\n", 2);
			ft_export(program, (char *[]){"export", ft_strjoin("?=",
					ft_itoa(2)), NULL});
			return (1);
		}
		current = current->next;
	}
	return (0);
}
