/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperez-a <bperez-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 11:37:35 by bperez-a          #+#    #+#             */
/*   Updated: 2024/05/02 12:05:27 by bperez-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void run(char *input)
{
	t_token		*head_token;
	t_command	*head_command;

	head_token = lex(input);
	head_command = parse(head_token);
	(void)head_command;
	//expand(head_command);
	//execute(head_command);
}

int	main(void)
{
	char		*input;

	while (1)
	{
		input = readline("minishell$ ");
		test_display(input);
		//run(input);
		add_history(input);
	}
}



