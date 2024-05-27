/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   freeing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperez-a <bperez-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 00:42:25 by bperez-a          #+#    #+#             */
/*   Updated: 2024/05/27 10:49:53 by bperez-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	free_redirects(t_redirection *redirects)
{
	t_redirection	*current_redirect;
	t_redirection	*next_redirect;

	current_redirect = redirects;
	while (current_redirect)
	{
		next_redirect = current_redirect->next;
		if (current_redirect->file)
			free(current_redirect->file);
		free(current_redirect);
		current_redirect = next_redirect;
	}
}

void	free_commands(t_command *commands)
{
	t_command	*current_command;
	t_command	*next_command;

	current_command = commands;
	while (current_command)
	{
		next_command = current_command->next;
		if (current_command->argv)
			ft_free_array(current_command->argv);
		if (current_command->redirects)
			free_redirects(current_command->redirects);
		free(current_command);
		current_command = next_command;
	}
}

void	free_tokens(t_token *tokens)
{
	t_token	*current_token;
	t_token	*next_token;

	current_token = tokens;
	while (current_token)
	{
		next_token = current_token->next;
		if (current_token->value)
			free(current_token->value);
		free(current_token);
		current_token = next_token;
	}
}

void	free_program(t_program *program)
{
	if (program->input)
		free(program->input);
	if (program->tokens)
		free_tokens(program->tokens);
	if (program->commands)
		free_commands(program->commands);
	if (program->envp)
		ft_free_array(program->envp);
	free(program);
}

void	free_and_exit(t_program *program, int exit_status)
{
	free_program(program);
	exit(exit_status);
}
