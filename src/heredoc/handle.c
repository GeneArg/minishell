/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperez-a <bperez-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 10:33:42 by bperez-a          #+#    #+#             */
/*   Updated: 2024/05/29 10:21:33 by bperez-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*get_delimiter(char *input)
{
	char	*end;
	size_t	len;
	char	*delimiter;
	char	*start;

	start = ft_strnstr(input, "<<", strlen(input));
	if (!start)
		return (NULL);
	start += 2;
	while (*start == ' ')
		start++;
	end = start;
	while (*end && *end != ' ' && *end != '\n')
		end++;
	len = end - start;
	delimiter = (char *)malloc(len + 1);
	if (!delimiter)
		return (NULL);
	ft_strlcpy(delimiter, start, len + 1);
	return (delimiter);
}

int	read_and_add_line(char **heredoc_content, size_t *total_len,
		char *delimiter)
{
	char	*line;
	size_t	del_len;
	size_t	line_len;

	del_len = ft_strlen(delimiter);
	line = readline("> ");
	if (line == NULL || ft_strncmp(line, delimiter, del_len) == 0)
	{
		free(line);
		return (1);
	}
	line_len = ft_strlen(line);
	*heredoc_content = (char *)ft_realloc(*heredoc_content, *total_len
			+ line_len + 2);
	if (*heredoc_content == NULL)
	{
		free(line);
		return (2);
	}
	ft_strlcpy(*heredoc_content + *total_len, line, line_len + 1);
	*total_len += line_len;
	(*heredoc_content)[(*total_len)++] = '\n';
	(*heredoc_content)[*total_len] = '\0';
	free(line);
	return (0);
}

char	*read_heredoc_input(char *delimiter)
{
	char	*heredoc_content;
	size_t	total_len;
	int		res;

	heredoc_content = NULL;
	total_len = 0;
	while (1)
	{
		res = read_and_add_line(&heredoc_content, &total_len, delimiter);
		if (res == 1)
			break ;
		if (res == 2)
		{
			free(heredoc_content);
			return (NULL);
		}
	}
	if (heredoc_content == NULL)
	{
		heredoc_content = (char *)malloc(1);
		if (heredoc_content == NULL)
			return (NULL);
		heredoc_content[0] = '\0';
	}
	return (heredoc_content);
}

char	*replace_heredoc_with_filename(char *input, char *delimiter,
		char *filename)
{
	char	*start;
	char	*end;
	size_t	new_input_len;
	char	*new_input;

	start = ft_strnstr(input, "<<", ft_strlen(input));
	if (!start)
		return (NULL);
	end = start + 2;
	while (*end == ' ')
		end++;
	end += ft_strlen(delimiter);
	while (*end == ' ' || *end == '\n')
		end++;
	new_input_len = ft_strlen(input) - (end - start) + ft_strlen(filename) + 1;
	new_input = (char *)malloc(new_input_len);
	if (!new_input)
		return (NULL);
	ft_strlcpy(new_input, input, start - input + 1);
	ft_strlcat(new_input, filename, new_input_len);
	ft_strlcat(new_input, end, new_input_len);
	return (new_input);
}

char	*handle_heredoc(char *input)
{
	char	*delimiter;
	char	*heredoc_content;
	char	*tmp_filename;
	int		fd;
	char	*updated_input;

	tmp_filename = "/tmp/minishell_heredoc";
	delimiter = get_delimiter(input);
	heredoc_content = read_heredoc_input(delimiter);
	if (!heredoc_content)
	{
		free(delimiter);
		return (NULL);
	}
	fd = open(tmp_filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	write(fd, heredoc_content, strlen(heredoc_content));
	close(fd);
	updated_input = replace_heredoc_with_filename(input, delimiter,
			tmp_filename);
	free(delimiter);
	free(heredoc_content);
	return (updated_input);
}
