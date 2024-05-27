/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperez-a <bperez-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 15:29:48 by bperez-a          #+#    #+#             */
/*   Updated: 2024/05/27 11:17:36 by bperez-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	check_initial_syntax(t_token *token, t_program **program)
{
	if (!token)
		return (FAILURE);
	if (token->type == TOKEN_PIPE)
	{
		ft_error(program, NULL, "syntax error near unexpected token `|'", 2);
		return (FAILURE);
	}
	return (SUCCESS);
}

int	handle_special_cases(t_token **current)
{
	if ((*current)->type == TOKEN_PIPE && (*current)->next
		&& ((*current)->next->type == TOKEN_REDIRECT_IN
			|| (*current)->next->type == TOKEN_REDIRECT_OUT))
	{
		*current = (*current)->next;
		return (FAILURE);
	}
	return (SUCCESS);
}

int	check_errors_and_continue(t_token *current, t_program **program)
{
	char	*error_msg;

	if (current->type != TOKEN_WORD && current->next == NULL)
	{
		ft_error(program, NULL,
			"syntax error near unexpected token `newline'", 2);
		return (FAILURE);
	}
	if (current->type != TOKEN_WORD && current->next->type != TOKEN_WORD)
	{
		error_msg = ft_strjoin("syntax error near unexpected token `",
				current->next->value);
		error_msg = ft_strjoin(error_msg, "'");
		ft_error(program, NULL, error_msg, 2);
		free(error_msg);
		return (FAILURE);
	}
	return (SUCCESS);
}

int	check_main_syntax(t_token *token, t_program **program)
{
	t_token	*current;

	current = token;
	while (current)
	{
		if (handle_special_cases(&current))
			continue ;
		if (check_errors_and_continue(current, program))
			return (FAILURE);
		current = current->next;
	}
	return (SUCCESS);
}

int	check_syntax(t_token *token, t_program **program)
{
	if (check_initial_syntax(token, program))
		return (FAILURE);
	return (check_main_syntax(token, program));
}
