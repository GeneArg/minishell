/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperez-a <bperez-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 14:35:37 by bperez-a          #+#    #+#             */
/*   Updated: 2024/05/17 11:28:49 by bperez-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_command	*init_command(void)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->argv = NULL;
	cmd->redirects = NULL;
	cmd->append = 0;
	cmd->next = NULL;
	cmd->flag_error = 0;
	return (cmd);
}


char	**create_and_copy_new_argv(t_command **cmd, char *new_arg)
{
	int		current_arg_count;
	char	**updated_argv;

	current_arg_count = 0;
	if ((*cmd)->argv)
	{
		while ((*cmd)->argv[current_arg_count])
			current_arg_count++;
	}
	updated_argv = (char **)malloc(sizeof(char *) * (current_arg_count + 2));
	for (int i = 0; i < current_arg_count; i++)
		updated_argv[i] = (*cmd)->argv[i];
	updated_argv[current_arg_count] = ft_strdup(new_arg);
	updated_argv[current_arg_count + 1] = NULL;
	return (updated_argv);
}

void	append_argument(t_command **cmd, char *arg)
{
	int		i;
	int		j;
	char	*new_arg;
	char	quote;
	char	**updated_argv;

	i = 0;
	j = 0;
	new_arg = (char *)malloc(strlen(arg) + 1);
	if (new_arg == NULL)
	{
		fprintf(stderr, "Memory allocation failed\n");
		return ;
	}
	while (arg[i] != '\0')
	{
		if (arg[i] == '\'' || arg[i] == '\"')
		{
			quote = arg[i];
			new_arg[j++] = arg[i++]; // Copy opening quote
			while (arg[i] != '\0' && arg[i] != quote)
			{
				new_arg[j++] = arg[i++];
			}
			if (arg[i] == quote)
			{
				new_arg[j++] = arg[i++]; // Copy closing quote
			}
		}
		else
		{
			new_arg[j++] = arg[i++];
		}
	}
	new_arg[j] = '\0';
	updated_argv = create_and_copy_new_argv(cmd, new_arg);
	if ((*cmd)->argv)
		free((*cmd)->argv);
	(*cmd)->argv = updated_argv;
	free(new_arg);
}

t_redirection	*create_new_redirection(char *file, RedirectionType type)
{
	t_redirection	*new_redirection;

	new_redirection = malloc(sizeof(t_redirection));
	if (!new_redirection)
	{
		ft_putstr_fd("Failed to allocate memory for redirection\n", 2);
		return (NULL);
	}
	new_redirection->file = ft_strdup(file);
	if (!new_redirection->file)
	{
		ft_putstr_fd("Failed to allocate memory for file name\n", 2);
		free(new_redirection);
		return (NULL);
	}
	new_redirection->type = type;
	new_redirection->next = NULL;
	return (new_redirection);
}

void	handle_redirect(t_command **cmd, char *file, RedirectionType type)
{
	t_redirection	*new_redirection;
	t_redirection	*current_redirection;

	new_redirection = create_new_redirection(file, type);
	if (!new_redirection)
		return ;
	if (!(*cmd)->redirects)
		(*cmd)->redirects = new_redirection;
	else
	{
		current_redirection = (*cmd)->redirects;
		while (current_redirection->next)
			current_redirection = current_redirection->next;
		current_redirection->next = new_redirection;
	}
}



t_command	*parse(t_token *token)
{
	t_command	*head;
	t_command	*current_cmd;
	t_command	*new_cmd;

	head = NULL;
	current_cmd = NULL;
	while (token)
	{
		if (!head || token->type == TOKEN_PIPE)
		{
			new_cmd = init_command();
			if (!head)
			{
				head = new_cmd;
				current_cmd = head;
			}
			else
			{
				current_cmd->next = new_cmd;
				current_cmd = new_cmd;
			}
		}
		if (token->type == TOKEN_WORD)
			append_argument(&current_cmd, token->value);
		if (token->type == TOKEN_REDIRECT_IN)
		{
			token = token->next;
			if (!token || token->type != TOKEN_WORD)
				ft_putstr_fd("syntax error\n", 2);
			else
			{
				handle_redirect(&current_cmd, token->value, REDIRECT_IN);
			}
		}

		if (token->type == TOKEN_REDIRECT_OUT || token->type == TOKEN_REDIRECT_APPEND)
		{
			if (token->type == TOKEN_REDIRECT_APPEND)
				current_cmd->append = 1;
			token = token->next;
			if (!token || token->type != TOKEN_WORD)
				ft_putstr_fd("syntax error\n", 2);
			else
			{
				handle_redirect(&current_cmd, token->value, REDIRECT_OUT);
			}
		}

		token = token->next;
	}
	return (head);
}
