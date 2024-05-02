/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperez-a <bperez-a@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 11:37:35 by bperez-a          #+#    #+#             */
/*   Updated: 2024/05/03 00:46:26 by bperez-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_program	*init_program()
{
	t_program	*program;

	program = malloc(sizeof(t_program));
	program->input = NULL;
	program->tokens = NULL;
	program->commands = NULL;
	return (program);
}

void run(t_program *program)
{
	program->tokens = lex(program->input);
	program->commands = parse(program->tokens);
	(void)program->commands;
	//expand(program->commands);
	//execute(program->commands);
}

int	main(void)
{
	t_program	*program;

	program = init_program();

	while (1)
	{
		program->input = readline("minishell$ ");
		test_display(program);
		//run(program);
		add_history(program->input);
		free_program_nonpersistent_values(program);
	}
}
