/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperez-a <bperez-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 12:56:22 by bperez-a          #+#    #+#             */
/*   Updated: 2024/06/10 09:27:27 by bperez-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	expand_args_helper(char **arg, char **envp)
{
	if (is_enclosed_in_single_quotes(*arg))
		trim_quotes(arg);
	else
	{
		trim_quotes(arg);
		replace_env_variables(arg, envp);
	}
}

void	expand_args(t_command *cmd, char **envp)
{
	int	i;

	if (!cmd || !cmd->argv)
		return ;
	i = 0;
	while (cmd->argv[i] != NULL)
	{
		expand_args_helper(&(cmd->argv[i]), envp);
		i++;
	}
	remove_empty_args(&(cmd->argv));
}

void	expand_redirects_helper(t_redirection *redir, char **envp)
{
	if (is_enclosed_in_single_quotes(redir->file))
		trim_quotes(&(redir->file));
	else
	{
		trim_quotes(&(redir->file));
		replace_env_variables(&(redir->file), envp);
	}
}

void	expand_redirects(t_redirection *redirects, char **envp)
{
	while (redirects != NULL)
	{
		expand_redirects_helper(redirects, envp);
		redirects = redirects->next;
	}
}

void	expand(t_command *commands, char **envp)
{
	t_command	*current;

	current = commands;
	while (current != NULL)
	{
		expand_args(current, envp);
		expand_redirects(current->redirects, envp);
		current = current->next;
	}
}
