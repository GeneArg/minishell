/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   freeing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperez-a <bperez-a@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 00:42:25 by bperez-a          #+#    #+#             */
/*   Updated: 2024/05/11 08:28:08 by bperez-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void free_program_nonpersistent_values(t_program *program)
{
	t_token		*token;
	t_token		*next_token;
	t_command	*command;
	t_command	*next_command;
	int			i;

	if (program->input)
		free(program->input);
	token = program->tokens;
	while (token)
	{
		next_token = token->next;
		free(token->value);
		free(token);
		token = next_token;
	}
	command = program->commands;
	while (command)
	{
		next_command = command->next;
		i = 0;
		if (command->argv)
		{
			while (command->argv[i])
				free(command->argv[i++]);
		}
		if (command->redirect_in)
			free(command->redirect_in);
		if (command->redirect_out)
			free(command->redirect_out);
		if (command->heredoc_content)
			free(command->heredoc_content);
		free(command);
		command = next_command;
	}
}

void free_program(t_program *program, int exit_status)
{
	free_program_nonpersistent_values(program);
	//free rest
	free(program);
	exit(exit_status);
}
