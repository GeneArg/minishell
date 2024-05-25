/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eagranat <eagranat@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 09:35:32 by bperez-a          #+#    #+#             */
/*   Updated: 2024/05/25 12:43:33 by eagranat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


void handle_sigint_blocking(int sig)
{
    (void)sig;
	write(1, "\n", 1);
}

void handle_sigint_non_blocking(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	// rl_replace_line("", 0);
	rl_redisplay();
}

void handle_sigquit(int sig)
{
	(void)sig;
    write(1, "Quit (core dumped)\n", 19);
}

