/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pretty.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperez-a <bperez-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 15:31:35 by bperez-a          #+#    #+#             */
/*   Updated: 2024/05/16 11:03:45 by bperez-a         ###   ########.fr       */
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


char	*ft_prompt(t_program *program)
{
    char	*prompt;
    char	*pwd;
    char	*home;
    char	*user;
    char	*temp;

    pwd = find_env_var_value(program->envp, "PWD");
    home = find_env_var_value(program->envp, "HOME");
    user = find_env_var_value(program->envp, "USER");

    char *color_user = "\033[1;34m"; // Blue
    char *color_path = "\033[0;35m"; // Purple (zsh)
    char *color_reset = "\033[0m"; // Reset color

    prompt = ft_strjoin(user, "@");
    temp = prompt;
    prompt = ft_strjoin(prompt, ":");
    free(temp);

    if (!ft_strncmp(pwd, home, ft_strlen(home)))
    {
        temp = prompt;
        prompt = ft_strjoin(prompt, "~");
        free(temp);

        temp = prompt;
        prompt = ft_strjoin(prompt, pwd + ft_strlen(home));
        free(temp);
    }
    else
    {
        temp = prompt;
        prompt = ft_strjoin(prompt, pwd);
        free(temp);
    }

    temp = prompt;
    prompt = ft_strjoin(prompt, " $ ");
    free(temp);

    temp = prompt;
    prompt = ft_strjoin(color_user, prompt);
    free(temp);

    temp = prompt;
    prompt = ft_strjoin(prompt, color_reset);
    free(temp);

    temp = prompt;
    prompt = ft_strjoin(color_path, prompt);
    free(temp);

    temp = prompt;
    prompt = ft_strjoin(prompt, color_reset);
    free(temp);

    return (prompt);
}
