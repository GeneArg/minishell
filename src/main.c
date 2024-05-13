/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperez-a <bperez-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 11:37:35 by bperez-a          #+#    #+#             */
/*   Updated: 2024/05/13 15:49:17 by bperez-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"





void	run(t_program **program)
{
	(*program)->tokens = lex((*program)->input);
	//printf("LEXER OUTPUT\n");
	//display_lexer_output((*program)->tokens);
	if (check_syntax((*program)->tokens, program))
		return ;
	(*program)->commands = parse((*program)->tokens);
	//printf("PARSER OUTPUT\n");
	//display_args((*program)->commands);
	expand((*program)->commands, (*program)->envp);
	//printf("EXPANDER OUTPUT\n");
	//display_args((*program)->commands);
	execute(program);
}

void	handle_sigint(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}



int	main(int argc, char **argv, char **envp)
{
	t_program	*program;

	program = init_program();
	program->envp = ft_copy_array(envp);
	ft_export(&program, (char *[]){"export", ft_strjoin("?=", ft_itoa(0)),
		NULL});
	increase_shlvl(&program);
	init_pwd(&program);
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	// print_welcome_msg();

	while (1)
	{
		program->input = readline(ft_prompt(program));
		run(&program);
		add_history(program->input);
	}
	(void)argc;
	(void)argv;
}
