/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eagranat <eagranat@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 11:37:35 by bperez-a          #+#    #+#             */
/*   Updated: 2024/05/14 19:30:37 by eagranat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


int g_in_blocking_mode = 0;  // Global variable to check blocking mode



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

void handle_sigint(int sig)
{
    (void)sig;
	if (!g_in_blocking_mode)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else
		write(1, "\n", 1);
}

void handle_sigquit(int sig)
{
    (void)sig;
    if (g_in_blocking_mode) {
        write(1, "Quit (core dumped)\n", 19);
    }
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
	// print_welcome_msg();

	while (1)
	{
		g_in_blocking_mode = 0;
		signal(SIGQUIT, SIG_IGN);
		program->input = readline(ft_prompt(program));
		g_in_blocking_mode = 1;
		signal(SIGQUIT, handle_sigquit);
		if (!program->input)
		{
			ft_putstr_fd("exit\n", 1);
			exit(1);
		}
		run(&program);
		add_history(program->input);
	}
	(void)argc;
	(void)argv;
}
