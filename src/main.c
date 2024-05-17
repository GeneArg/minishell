/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperez-a <bperez-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 11:37:35 by bperez-a          #+#    #+#             */
/*   Updated: 2024/05/17 11:18:46 by bperez-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	run(t_program **program)
{
	(*program)->tokens = lex((*program)->input);
	if (check_syntax((*program)->tokens, program))
	{
		free_tokens((*program)->tokens);
		(*program)->tokens = NULL;
		free((*program)->input);
		(*program)->input = NULL;
		return ;
	}
	//display_lexer_output((*program)->tokens);
	(*program)->commands = parse((*program)->tokens);
	//display_args((*program)->commands);
	free_tokens((*program)->tokens);
	(*program)->tokens = NULL;
	expand((*program)->commands, (*program)->envp);
	execute(program);
}


int	main(int argc, char **argv, char **envp)
{
	t_program	*program;
	char		*prompt;

	program = init_program();
	program->envp = ft_copy_array(envp);
	ft_export(&program, (char *[]){"export", "?=0", NULL});
	increase_shlvl(&program);
	init_pwd(&program);
	// print_welcome_msg();
	while (1)
	{
		signal(SIGQUIT, SIG_IGN);
		signal(SIGINT, handle_sigint_non_blocking);
		prompt = ft_prompt(program);
		
		program->input = readline(prompt);
		free(prompt);
		if (!program->input)
		{
			ft_putstr_fd("exit\n", 1);
			free_and_exit(program, 1);
		}
		if (is_heredoc(program->input))
		{
			program->input = handle_heredoc(program->input);
			if (!program->input)
			{
				free_program(program);
				continue ;
			}
		}
		signal(SIGQUIT, handle_sigquit);
		signal(SIGINT, handle_sigint_blocking);
		if (ft_strlen(program->input) == 0)
		{
			free(program->input);
			continue ;
		}
		run(&program);
		add_history(program->input);
		free(program->input);
		free_commands(program->commands);
	}
	(void)argc;
	(void)argv;
}
