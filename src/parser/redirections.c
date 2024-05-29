/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperez-a <bperez-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 10:34:35 by bperez-a          #+#    #+#             */
/*   Updated: 2024/05/29 10:34:39 by bperez-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_redirection	*create_new_redirection(char *file, t_redirection_type type)
{
	t_redirection	*new_redirection;

	new_redirection = malloc(sizeof(t_redirection));
	if (!new_redirection)
	{
		ft_putstr_fd("Failed to allocate memory for redirection\n", 2);
		return (NULL);
	}
	new_redirection->file = ft_strdup(file);
	if (!new_redirection->file)
	{
		ft_putstr_fd("Failed to allocate memory for file name\n", 2);
		free(new_redirection);
		return (NULL);
	}
	new_redirection->type = type;
	new_redirection->next = NULL;
	return (new_redirection);
}

void	handle_redirect(t_command **cmd, char *file, t_redirection_type type)
{
	t_redirection	*new_redirection;
	t_redirection	*current_redirection;

	new_redirection = create_new_redirection(file, type);
	if (!new_redirection)
		return ;
	if (!(*cmd)->redirects)
		(*cmd)->redirects = new_redirection;
	else
	{
		current_redirection = (*cmd)->redirects;
		while (current_redirection->next)
			current_redirection = current_redirection->next;
		current_redirection->next = new_redirection;
	}
}

void	handle_token_redirect_in(t_token **token, t_command **current_cmd)
{
	*token = (*token)->next;
	if (!(*token) || (*token)->type != TOKEN_WORD)
		ft_putstr_fd("syntax error\n", 2);
	else
		handle_redirect(current_cmd, (*token)->value, REDIRECT_IN);
}

void	handle_token_redirect_out(t_token **token, t_command **current_cmd)
{
	if ((*token)->type == TOKEN_REDIRECT_APPEND)
		(*current_cmd)->append = 1;
	*token = (*token)->next;
	if (!(*token) || (*token)->type != TOKEN_WORD)
		ft_putstr_fd("syntax error\n", 2);
	else
		handle_redirect(current_cmd, (*token)->value, REDIRECT_OUT);
}
