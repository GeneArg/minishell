/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eagranat <eagranat@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 11:37:35 by bperez-a          #+#    #+#             */
/*   Updated: 2024/05/03 17:19:27 by eagranat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	print_welcome_msg(void)
{
	ft_putstr_fd("                                        \n", 2);
	ft_putstr_fd("               ▄█████████▄              \n", 2);
	ft_putstr_fd("            ▄███████████████▄           \n", 2);
	ft_putstr_fd("         ██████▀   ██   ▀███████        \n", 2);
	ft_putstr_fd("      ███   ▀███   ██   ███▀   ███      \n", 2);
	ft_putstr_fd("     ██████   ██   ██   ██   ██████     \n", 2);
	ft_putstr_fd("     ██   ██   ██  ██  ██   ██   ██     \n", 2);
	ft_putstr_fd("    ███    ██  ██  ██  ██  ██    ███    \n", 2);
	ft_putstr_fd("    ██ ██   ██  █  ██  █  ██   ██ ██    \n", 2);
	ft_putstr_fd("    ██  ███  ██ ██ ██ ██ ██  ███  ██    \n", 2);
	ft_putstr_fd("    ██    ██  ██ █ ██ █ ██  ██    ██    \n", 2);
	ft_putstr_fd("    ████▄   ██ █  █  █  █ ██   ▄████    \n", 2);
	ft_putstr_fd("       ████   █          █   ████       \n", 2);
	ft_putstr_fd("          ██                ██          \n", 2);
	ft_putstr_fd("          ████████▄  ▄████████          \n", 2);
	ft_putstr_fd("                  ▀██▀                  \n", 2);
	ft_putstr_fd("           _       _     _          _ _ \n", 2);
	ft_putstr_fd(" _ __ ___ (_)_ __ (_)___| |__   ___| | |\n", 2);
	ft_putstr_fd("| '_ ` _ \\| | '_ \\| / __| '_ \\ / _ \\ | |\n", 2);
	ft_putstr_fd("| | | | | | | | | | \\__ \\ | | |  __/ | |\n", 2);
	ft_putstr_fd("|_| |_| |_|_|_| |_|_|___/_| |_|\\___|_|_|\n\n", 2);
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
		program->input = readline("minishell$ ");
		//test_display(program);
		run(program);
		add_history(program->input);
		free_program_nonpersistent_values(program);
	}
	(void)argc;
	(void)argv;
}
