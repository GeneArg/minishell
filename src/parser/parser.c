/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eagranat <eagranat@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 14:35:37 by bperez-a          #+#    #+#             */
/*   Updated: 2024/05/07 22:10:43 by eagranat         ###   ########.fr       */
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
	cmd->redirect_in = NULL;
	cmd->redirect_out = NULL;
	cmd->append = 0;
	cmd->heredoc_content = NULL;
	cmd->next = NULL;
	return (cmd);
}

// void	append_argument(t_command **cmd, char *arg)
// {
// 	int		argc;
// 	char	**new_argv;

// 	argc = 0;
// 	if ((*cmd)->argv)
// 	{
// 		while ((*cmd)->argv[argc])
// 			argc++;
// 	}
// 	new_argv = (char **)malloc(sizeof(char *) * (argc + 2));
// 	for (int i = 0; i < argc; i++)
// 		new_argv[i] = (*cmd)->argv[i];
// 	new_argv[argc] = ft_strdup(arg);
// 	new_argv[argc + 1] = NULL;
// 	if ((*cmd)->argv)
// 		free((*cmd)->argv);
// 	(*cmd)->argv = new_argv;
// }

void	append_argument(t_command **cmd, char *arg)
{
	int		argc;
	char	**new_argv;
	char	*new_arg;
	int		i;
	int		j;
	char	quote;

	new_arg = malloc(strlen(arg) + 1);
	i = 0;
	j = 0;
	while (arg[i] != '\0')
	{
		if (arg[i] == '\'' || arg[i] == '\"')
		{
			quote = arg[i++];
			while (arg[i] != quote)
			{
				new_arg[j++] = arg[i++];
			}
			i++; // Skip the closing quote
		}
		else
		{
			new_arg[j++] = arg[i++];
		}
	}
	new_arg[j] = '\0';
	argc = 0;
	if ((*cmd)->argv)
	{
		while ((*cmd)->argv[argc])
			argc++;
	}
	new_argv = (char **)malloc(sizeof(char *) * (argc + 2));
	for (int i = 0; i < argc; i++)
		new_argv[i] = (*cmd)->argv[i];
	new_argv[argc] = ft_strdup(new_arg);
	new_argv[argc + 1] = NULL;
	if ((*cmd)->argv)
		free((*cmd)->argv);
	(*cmd)->argv = new_argv;
	free(new_arg);
}

t_command	*parse(t_token *token)
{
	t_command	*head;
	t_command	*current_cmd;
	t_command	*new_cmd;
	char		*heredoc_delimiter;

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
			if (token)
				current_cmd->redirect_in = ft_strdup(token->value);
		}
		if (token->type == TOKEN_REDIRECT_OUT)
		{
			token = token->next;
			if (token)
				current_cmd->redirect_out = ft_strdup(token->value);
		}
		if (token->type == TOKEN_REDIRECT_APPEND)
		{
			token = token->next;
			if (token)
			{
				current_cmd->redirect_out = ft_strdup(token->value);
				current_cmd->append = 1;
			}
		}
		if (token->type == TOKEN_HEREDOC)
		{
			token = token->next;
			if (token)
			{
				heredoc_delimiter = ft_strdup(token->value);
				token = token->next;
				while (token->value && strcmp(token->value,
						heredoc_delimiter) != 0)
				{
					if (current_cmd->heredoc_content)
						current_cmd->heredoc_content = ft_strjoin(current_cmd->heredoc_content,
								" ");
					else
						current_cmd->heredoc_content = ft_strdup("");
					current_cmd->heredoc_content = ft_strjoin(current_cmd->heredoc_content,
							token->value);
					token = token->next;
				}
			}
		}
		token = token->next;
	}
	return (head);
}
