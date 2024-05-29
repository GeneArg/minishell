/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperez-a <bperez-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 12:40:01 by bperez-a          #+#    #+#             */
/*   Updated: 2024/05/29 12:44:02 by bperez-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

bool	is_enclosed_in_single_quotes(char *arg)
{
	int	len;

	len = ft_strlen(arg);
	return (len >= 2 && arg[0] == '\'' && arg[len - 1] == '\'');
}

bool	toggle_quote_state(char c, bool *inside_quotes, char *current_quote)
{
	if ((c == '\'' || c == '\"') && (!(*inside_quotes) || *current_quote == c))
	{
		if (!(*inside_quotes))
		{
			*inside_quotes = true;
			*current_quote = c;
		}
		else
		{
			*inside_quotes = false;
			*current_quote = 0;
		}
		return (true);
	}
	return (false);
}

void	trim_quotes_helper(char *input, char *output)
{
	int		i;
	int		j;
	bool	inside_quotes;
	char	current_quote;

	i = 0;
	j = 0;
	inside_quotes = false;
	current_quote = 0;
	while (input[i] != '\0')
	{
		if (!toggle_quote_state(input[i], &inside_quotes, &current_quote))
		{
			output[j++] = input[i];
		}
		i++;
	}
	output[j] = '\0';
}

void	trim_quotes(char **arg)
{
	char	*input;
	char	*output;

	input = *arg;
	output = malloc(ft_strlen(input) + 1);
	if (!output)
	{
		perror("Memory allocation failed");
		return ;
	}
	trim_quotes_helper(input, output);
	free(*arg);
	*arg = output;
}
