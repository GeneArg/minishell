/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eagranat <eagranat@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 11:37:35 by bperez-a          #+#    #+#             */
/*   Updated: 2024/05/04 00:27:21 by eagranat         ###   ########.fr       */
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

void run(t_program *program)
{
	program->tokens = lex(program->input);
	program->commands = parse(program->tokens);
	//expand(program->commands);
	execute(program);
}


int	main(int argc, char **argv, char **envp)
{
	t_program	*program;

	program = init_program();
	program->envp = envp;
	while (1)
	{
		program->input = readline("\033[1;31mminishell$ \033[0m");
		//test_display(program);
		run(program);
		add_history(program->input);
		free_program_nonpersistent_values(program);
	}
	(void)argc;
	(void)argv;
}
