/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperez-a <bperez-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 11:37:35 by bperez-a          #+#    #+#             */
/*   Updated: 2024/05/06 13:08:10 by bperez-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	print_welcome_msg(void)
{
    ft_putstr_fd("\033[1;33m                                    \n\033[0m", 2);
    ft_putstr_fd("\033[1;33m             ▄█████████▄            \n\033[0m", 2);
    ft_putstr_fd("\033[1;33m          ▄███████████████▄         \n\033[0m", 2);
    ft_putstr_fd("\033[1;33m       ██████▀   ██   ▀███████      \n\033[0m", 2);
    ft_putstr_fd("\033[1;33m    ███   ▀███   ██   ███▀   ███    \n\033[0m", 2);
    ft_putstr_fd("\033[1;33m   ██████   ██   ██   ██   ██████   \n\033[0m", 2);
    ft_putstr_fd("\033[1;33m   ██   ██   ██  ██  ██   ██   ██   \n\033[0m", 2);
    ft_putstr_fd("\033[1;33m  ███    ██  ██  ██  ██  ██    ███  \n\033[0m", 2);
    ft_putstr_fd("\033[1;33m  ██ ██   ██  █  ██  █  ██   ██ ██  \n\033[0m", 2);
    ft_putstr_fd("\033[1;33m  ██  ███  ██ ██ ██ ██ ██  ███  ██  \n\033[0m", 2);
    ft_putstr_fd("\033[1;33m  ██    ██  ██ █ ██ █ ██  ██    ██  \n\033[0m", 2);
    ft_putstr_fd("\033[1;33m  ████▄   ██ █  █  █  █ ██   ▄████  \n\033[0m", 2);
    ft_putstr_fd("\033[1;33m     ████   █          █   ████     \n\033[0m", 2);
    ft_putstr_fd("\033[1;33m        ██                ██        \n\033[0m", 2);
    ft_putstr_fd("\033[1;33m        ████████▄  ▄████████        \n\033[0m", 2);
    ft_putstr_fd("\033[1;33m    eagranat    ▀██▀    bperez-a    \n\033[0m", 2);
    ft_putstr_fd("\033[1;31m           _       _     _          _ _ \n\033[0m", 2);
    ft_putstr_fd("\033[1;32m _ __ ___ (_)_ __ (_)___| |__   ___| | |\n\033[0m", 2);
    ft_putstr_fd("\033[1;33m| '_ ` _ \\| | '_ \\| / __| '_ \\ / _ \\ | |\n\033[0m", 2);
    ft_putstr_fd("\033[1;34m| | | | | | | | | | \\__ \\ | | |  __/ | |\n\033[0m", 2);
    ft_putstr_fd("\033[1;35m|_| |_| |_|_|_| |_|_|___/_| |_|\\___|_|_|\n\n\033[0m", 2);
}

t_program	*init_program()
{
	t_program	*program;

	program = malloc(sizeof(t_program));
	program->input = NULL;
	program->tokens = NULL;
	program->commands = NULL;
	print_welcome_msg();
	return (program);
}

void run(t_program **program)
{
	(*program)->tokens = lex((*program)->input);
	(*program)->commands = parse((*program)->tokens);
	expand((*program)->commands, (*program)->envp);
	//test_display(*program);
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
	program->test = "FAILURE";
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);

	while (1)
	{
		
		program->input = readline("\033[1;31mminishell$ \033[0m");
		//test_display(program);
		run(&program);
		add_history(program->input);
		free_program_nonpersistent_values(program);
		
	}
	(void)argc;
	(void)argv;
}
