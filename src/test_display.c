/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_display.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperez-a <bperez-a@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 11:47:48 by bperez-a          #+#    #+#             */
/*   Updated: 2024/05/03 00:24:54 by bperez-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void test_display(t_program *program)
{
	t_token		*current;
	t_command	*current_command;

	printf("INPUT: %s\n", program->input);
	program->tokens = lex(program->input);
	current = program->tokens;
	printf("\nLEXER OUTPUT:\n");
	while (current)
	{
		printf("Type: %d, Value: %s\n", current->type, current->value);
		current = current->next;
	}
	printf("\nPARSER OUTPUT:\n");
	program->commands = parse(program->tokens);
	current_command = program->commands;
	while (current_command)
	{
		printf("Command: %s\n", *(current_command->argv));
		current_command->argv++;
		while (*(current_command->argv))
		{
			printf("Arg: %s\n", *(current_command->argv));
			current_command->argv++;
		}
		if (current_command->redirect_in)
			printf("Redirect in: %s\n", current_command->redirect_in);
		if (current_command->redirect_out)
			printf("Redirect out: %s\n", current_command->redirect_out);
		if (current_command->heredoc_content)
			printf("Heredoc content: %s\n", current_command->heredoc_content);
		if (current_command->append)
			printf("Append: %d\n", current_command->append);
		current_command = current_command->next;
		printf("\n");
	}
}
