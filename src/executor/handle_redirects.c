/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirects.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperez-a <bperez-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 13:16:59 by bperez-a          #+#    #+#             */
/*   Updated: 2024/06/04 13:19:47 by bperez-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	handle_input_redirection(t_redirection *current_redirection)
{
	int	fd;

	fd = open(current_redirection->file, O_RDONLY);
	if (fd < 0)
	{
		perror(current_redirection->file);
		exit(EXIT_FAILURE);
	}
	dup2(fd, 0);
	close(fd);
}

void	handle_output_redirection(t_redirection *current_redirection,
		int append)
{
	int	fd;

	if (append)
	{
		fd = open(current_redirection->file, O_WRONLY | O_CREAT | O_APPEND,
				0644);
	}
	else
	{
		fd = open(current_redirection->file, O_WRONLY | O_CREAT | O_TRUNC,
				0644);
	}
	if (fd < 0)
	{
		perror(current_redirection->file);
		exit(EXIT_FAILURE);
	}
	dup2(fd, 1);
	close(fd);
}

void	handle_redirection(t_command *cmd)
{
	t_redirection	*current_redirection;

	current_redirection = cmd->redirects;
	while (current_redirection)
	{
		if (current_redirection->type == REDIRECT_IN)
			handle_input_redirection(current_redirection);
		else if (current_redirection->type == REDIRECT_OUT)
			handle_output_redirection(current_redirection, cmd->append);
		current_redirection = current_redirection->next;
	}
}
