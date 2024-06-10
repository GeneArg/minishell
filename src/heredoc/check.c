/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperez-a <bperez-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 09:46:04 by bperez-a          #+#    #+#             */
/*   Updated: 2024/06/10 11:20:50 by bperez-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*generate_unique_filename(void)
{
	static int	i = 0;
	char		*number;
	char		*filename;

	number = ft_itoa(i++);
	filename = ft_strjoin("/tmp/minishell_heredoc", number);
	free(number);
	return (filename);
}

void	check_quotes(char **ptr, bool *in_single_quote, bool *in_double_quote)
{
	if (**ptr == '\'' && !(*in_double_quote))
		*in_single_quote = !(*in_single_quote);
	else if (**ptr == '\"' && !(*in_single_quote))
		*in_double_quote = !(*in_double_quote);
}

bool	detect_heredoc(char **ptr, bool in_single_quote, bool in_double_quote)
{
	if (**ptr == '<' && !in_single_quote && !in_double_quote)
	{
		if (*(*ptr + 1) == '<')
		{
			*ptr += 2;
			while (**ptr && isspace(**ptr))
				(*ptr)++;
			if (**ptr && !isspace(**ptr))
				return (true);
			else
				return (false);
		}
	}
	return (false);
}

bool	is_heredoc(char *input)
{
	bool	in_single_quote;
	bool	in_double_quote;
	char	*ptr;

	in_single_quote = false;
	in_double_quote = false;
	ptr = input;
	while (*ptr)
	{
		check_quotes(&ptr, &in_single_quote, &in_double_quote);
		if (detect_heredoc(&ptr, in_single_quote, in_double_quote))
			return (true);
		ptr++;
	}
	return (false);
}
