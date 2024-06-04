/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperez-a <bperez-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 11:37:35 by bperez-a          #+#    #+#             */
/*   Updated: 2024/06/04 12:15:44 by bperez-a         ###   ########.fr       */
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
	(*program)->commands = parse((*program)->tokens);
	free_tokens((*program)->tokens);
	(*program)->tokens = NULL;
	expand((*program)->commands, (*program)->envp);
	execute_pipeline(program);
}

void	setup_and_read_prompt(t_program *program)
{
	char	*prompt;

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
}

void	process_input(t_program *program)
{
	if (is_heredoc(program->input))
	{
		program->input = handle_heredoc(program->input);
		if (!program->input)
		{
			free_program(program);
			return ;
		}
	}
	signal(SIGQUIT, handle_sigquit);
	signal(SIGINT, handle_sigint_blocking);
	if (ft_strlen(program->input) == 0)
	{
		free(program->input);
		return ;
	}
	run(&program);
	add_history(program->input);
	free(program->input);
	free_commands(program->commands);
	program->commands = NULL;
}

void	loop(t_program *program)
{
	setup_and_read_prompt(program);
	process_input(program);
}

int	main(int argc, char **argv, char **envp)
{
	t_program	*program;

	(void)argc;
	(void)argv;
	program = init_program(envp);
	while (1)
		loop(program);
}
