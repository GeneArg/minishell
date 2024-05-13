/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperez-a <bperez-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 14:35:37 by bperez-a          #+#    #+#             */
/*   Updated: 2024/05/13 09:44:39 by bperez-a         ###   ########.fr       */
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
	cmd->flag_error = 0;
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

t_redirection	*create_new_redirection(char *file)
{
	t_redirection	*new_redirection;

	new_redirection = malloc(sizeof(t_redirection));
	if (!new_redirection)
	{
		ft_putstr_fd("Failed to allocate memory for redirection\n", 2);
		return (NULL);
	}
	new_redirection->file = strdup(file);
	if (!new_redirection->file)
	{
		ft_putstr_fd("Failed to allocate memory for file name\n", 2);
		free(new_redirection);
		return (NULL);
	}
	new_redirection->next = NULL;
	return (new_redirection);
}

void	handle_redirect_in(t_command **cmd, char *file)
{
	t_redirection	*new_redirection;
	t_redirection	*current_redirection;

	new_redirection = create_new_redirection(file);
	if (!new_redirection)
		return ;
	if (!(*cmd)->redirect_in)
		(*cmd)->redirect_in = new_redirection;
	else
	{
		current_redirection = (*cmd)->redirect_in;
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
	char		*heredoc_delimiter;
	char		*temp;

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
				if (token->value[0] == '\"')
				{
					while (token && token->value[strlen(token->value)
						- 1] != '\"')
					{
						temp = ft_strjoin(token->value, " ");
						free(token->value);
						token->value = temp;
						token = token->next;
						temp = ft_strjoin(token->value, token->next->value);
						free(token->next->value);
						token->next->value = temp;
					}
					token->value++;
					token->value[strlen(token->value) - 1] = '\0';
				}
				handle_redirect_in(&current_cmd, token->value);
			}
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
