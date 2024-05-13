/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperez-a <bperez-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 11:37:35 by bperez-a          #+#    #+#             */
/*   Updated: 2024/05/13 12:18:58 by bperez-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// static void	print_welcome_msg(void)
// {
//     ft_putstr_fd("\033[1;33m                                    \n\033[0m", 2);
//     ft_putstr_fd("\033[1;33m             ▄█████████▄            \n\033[0m", 2);
//     ft_putstr_fd("\033[1;33m          ▄███████████████▄         \n\033[0m", 2);
//     ft_putstr_fd("\033[1;33m       ██████▀   ██   ▀███████      \n\033[0m", 2);
//     ft_putstr_fd("\033[1;33m    ███   ▀███   ██   ███▀   ███    \n\033[0m", 2);
//     ft_putstr_fd("\033[1;33m   ██████   ██   ██   ██   ██████   \n\033[0m", 2);
//     ft_putstr_fd("\033[1;33m   ██   ██   ██  ██  ██   ██   ██   \n\033[0m", 2);
//     ft_putstr_fd("\033[1;33m  ███    ██  ██  ██  ██  ██    ███  \n\033[0m", 2);
//     ft_putstr_fd("\033[1;33m  ██ ██   ██  █  ██  █  ██   ██ ██  \n\033[0m", 2);
//     ft_putstr_fd("\033[1;33m  ██  ███  ██ ██ ██ ██ ██  ███  ██  \n\033[0m", 2);
//     ft_putstr_fd("\033[1;33m  ██    ██  ██ █ ██ █ ██  ██    ██  \n\033[0m", 2);
//     ft_putstr_fd("\033[1;33m  ████▄   ██ █  █  █  █ ██   ▄████  \n\033[0m", 2);
//     ft_putstr_fd("\033[1;33m     ████   █          █   ████     \n\033[0m", 2);
//     ft_putstr_fd("\033[1;33m        ██                ██        \n\033[0m", 2);
//     ft_putstr_fd("\033[1;33m        ████████▄  ▄████████        \n\033[0m", 2);
//     ft_putstr_fd("\033[1;33m    eagranat    ▀██▀    bperez-a    \n\033[0m", 2);
//     ft_putstr_fd("\033[1;31m           _       _     _          _ _ \n\033[0m", 2);
//     ft_putstr_fd("\033[1;32m _ __ ___ (_)_ __ (_)___| |__   ___| | |\n\033[0m", 2);
//     ft_putstr_fd("\033[1;33m| '_ ` _ \\| | '_ \\| / __| '_ \\ / _ \\ | |\n\033[0m", 2);
//     ft_putstr_fd("\033[1;34m| | | | | | | | | | \\__ \\ | | |  __/ | |\n\033[0m", 2);
//     ft_putstr_fd("\033[1;35m|_| |_| |_|_|_| |_|_|___/_| |_|\\___|_|_|\n\n\033[0m", 2);
// }

void	display_lexer_output(t_token *tokens)
{
	t_token	*tmp;

	tmp = tokens;
	while (tmp)
	{
		printf("type: %d, value: %s\n", tmp->type, tmp->value);
		tmp = tmp->next;
	}
}

void	display_args(t_command *commands)
{
	t_command	*tmp;
	int			i;

	tmp = commands;
	while (tmp)
	{
		i = 0;
		while (tmp->argv[i])
		{
			printf("args[%d]: %s\n", i, tmp->argv[i]);
			i++;
		}
		tmp = tmp->next;
	}
}
t_program	*init_program(void)
{
	t_program	*program;

	program = malloc(sizeof(t_program));
	program->input = NULL;
	program->tokens = NULL;
	program->commands = NULL;
	// print_welcome_msg();
	return (program);
}
int	check_syntax(t_token *token, t_program **program)
{
	t_token	*current;

	current = token;
	if (!current)
		return (1);
	if (current->type == TOKEN_PIPE)
	{
		ft_error(program, NULL, "syntax error near unexpected token `|'", 2);
		return (1);
	}
	while (current)
	{
		if (current->type == TOKEN_PIPE
			&& current->next && (current->next->type == TOKEN_REDIRECT_IN
				|| current->next->type == TOKEN_REDIRECT_OUT))
		{
			current = current->next;
			continue ;
		}
		if (current->type != TOKEN_WORD && current->next == NULL)
		{
			ft_error(program, NULL, "syntax error near unexpected token `newline'", 2);
			return (1);
		}
		if (current->type != TOKEN_WORD && current->next->type != TOKEN_WORD)
		{
			ft_putstr_fd("bash: syntax error near unexpected token `", 2);
			ft_putstr_fd(current->next->value, 2);
			ft_putstr_fd("'\n", 2);
			ft_export(program, (char *[]){"export", ft_strjoin("?=",
					ft_itoa(2)), NULL});
			return (1);
		}
		current = current->next;
	}
	return (0);
}

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
void	init_env(t_program **program, char **envp)
{
	(*program)->envp = ft_copy_array(envp);
}

void	init_pwd(t_program **program)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	ft_export(program, (char *[]){"export", ft_strjoin("PWD=", pwd), NULL});
	ft_export(program, (char *[]){"export", ft_strjoin("OLDPWD=", pwd), NULL});
	free(pwd);
}
char	*ft_prompt(t_program *program)
{
    char	*prompt;
    char	*pwd;
    char	*home;
    char	*user;
    char	*color_prompt;
    pwd = find_env_var_value(program->envp, "PWD");
    home = find_env_var_value(program->envp, "HOME");
    user = find_env_var_value(program->envp, "USER");
	if (!pwd || !home || !user)
		return (ft_strdup("\033[0;34myou broke our prompt $ "));
    char *color_user = "\033[1;34m"; // Blue
    char *color_path = "\033[0;35m"; // Purple (zsh)
    char *color_reset = "\033[0m"; // Reset color
    prompt = ft_strjoin(user, "@");
    prompt = ft_strjoin(prompt, ":");
    if (!ft_strncmp(pwd, home, ft_strlen(home)))
		prompt = ft_strjoin(ft_strjoin(prompt, "~"), pwd + ft_strlen(home));
    else
    	prompt = ft_strjoin(prompt, pwd);
    prompt = ft_strjoin(prompt, " $ ");
    color_prompt = ft_strjoin(color_user, prompt);
    color_prompt = ft_strjoin(color_prompt, color_reset);
    color_prompt = ft_strjoin(color_path, color_prompt);
    color_prompt = ft_strjoin(color_prompt, color_reset);
    return (color_prompt);
}

int	main(int argc, char **argv, char **envp)
{
	t_program	*program;

	program = init_program();
	program->envp = ft_copy_array(envp);
	ft_export(&program, (char *[]){"export", ft_strjoin("?=", ft_itoa(0)),
		NULL});
	init_pwd(&program);
	program->test = "FAILURE";
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		program->input = readline(ft_prompt(program));
		run(&program);
		add_history(program->input);
	}
	(void)argc;
	(void)argv;
}
