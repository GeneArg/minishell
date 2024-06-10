/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperez-a <bperez-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 10:33:42 by bperez-a          #+#    #+#             */
/*   Updated: 2024/06/10 11:21:07 by bperez-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*get_delimiter(char *input)
{
	char	*start;
	char	*end;
	size_t	len;
	char	*delimiter;

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

int	read_and_add_line(char **content, size_t *total_len, const char *delimiter)
{
	char	*line;
	size_t	del_len;
	size_t	line_len;

	line = readline("> ");
	if (!line)
		return (1);
	del_len = ft_strlen(delimiter);
	if (ft_strncmp(line, delimiter, del_len) == 0 && strlen(line) == del_len)
	{
		free(line);
		return (1);
	}
	line_len = strlen(line);
	*content = (char *)ft_realloc(*content, *total_len + line_len + 2);
	ft_strlcpy(*content + *total_len, line, line_len + 1);
	*total_len += line_len;
	(*content)[(*total_len)++] = '\n';
	(*content)[*total_len] = '\0';
	free(line);
	return (0);
}

char	*read_heredoc_input(const char *delimiter)
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
	if (heredoc_content)
		return (heredoc_content);
	else
		return (ft_strdup(""));
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
	new_input_len = ft_strlen(input) - (end - start) + ft_strlen(filename) + 5;
	new_input = (char *)malloc(new_input_len);
	if (!new_input)
		return (NULL);
	ft_strlcpy(new_input, input, start - input + 1);
	ft_strlcat(new_input, "< ", new_input_len);
	ft_strlcat(new_input, filename, new_input_len);
	ft_strlcat(new_input, " ", new_input_len);
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

	tmp_filename = generate_unique_filename();
	delimiter = get_delimiter(input);
	heredoc_content = read_heredoc_input(delimiter);
	if (!heredoc_content)
	{
		free(delimiter);
		return (NULL);
	}
	fd = open(tmp_filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	write(fd, heredoc_content, ft_strlen(heredoc_content));
	close(fd);
	updated_input = replace_heredoc_with_filename(input, delimiter,
			tmp_filename);
	free(delimiter);
	free(heredoc_content);
	free(tmp_filename);
	return (updated_input);
}
