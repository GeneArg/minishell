/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperez-a <bperez-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 10:33:42 by bperez-a          #+#    #+#             */
/*   Updated: 2024/05/17 13:35:18 by bperez-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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
		if (*ptr == '\'' && !in_double_quote)
		{
			in_single_quote = !in_single_quote;
		}
		else if (*ptr == '\"' && !in_single_quote)
		{
			in_double_quote = !in_double_quote;
		}
		else if (*ptr == '<' && !in_single_quote && !in_double_quote)
		{
			if (*(ptr + 1) == '<')
			{
				ptr += 2;
				// Skip any spaces after "<<"
				while (*ptr && isspace(*ptr))
				{
					ptr++;
				}
				// Check if there's a valid delimiter
				if (*ptr && !isspace(*ptr))
				{
					return (true);
				}
				else
				{
					return (false);
				}
			}
		}
		ptr++;
	}
	return (false);
}

// Function to extract the delimiter from the input
char	*get_delimiter(char *input)
{
	char	*end;
	size_t	len;
	char	*delimiter;

	char *start = strstr(input, "<<") + 2; // Skip the "<<"
	while (*start == ' ')
		start++; // Skip any spaces after "<<"
	end = start;
	while (*end && *end != ' ' && *end != '\n')
		end++; // Find the end of the delimiter
	len = end - start;
	delimiter = (char *)malloc(len + 1);
	strncpy(delimiter, start, len);
	delimiter[len] = '\0';
	return (delimiter);
}

// Function to read heredoc input until the delimiter is met
char	*read_heredoc_input(char *delimiter)
{
	char	*input_line;
	char	*heredoc_content;
	size_t	total_len;
	size_t	line_len;

	heredoc_content = NULL;
	total_len = 0;
	while (1)
	{
		input_line = readline("> ");
		if (input_line == NULL || strcmp(input_line, delimiter) == 0)
		{
			free(input_line);
			break ;
		}
		line_len = strlen(input_line);
		heredoc_content = realloc(heredoc_content, total_len + line_len + 2); // +2 for newline and null terminator
		if (heredoc_content == NULL)
		{
			perror("realloc");
			exit(EXIT_FAILURE);
		}
		strcpy(heredoc_content + total_len, input_line);
		total_len += line_len;
		heredoc_content[total_len++] = '\n';
		heredoc_content[total_len] = '\0';
		free(input_line);
	}
	// If heredoc_content is still NULL, allocate an empty string
	if (heredoc_content == NULL)
	{
		heredoc_content = malloc(1);
		if (heredoc_content == NULL)
		{
			perror("malloc");
			exit(EXIT_FAILURE);
		}
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

	start = strstr(input, "<<");
	end = start + 2;
	while (*end == ' ')
		end++;                // Skip any spaces after "<<"
	end += strlen(delimiter); // Skip the delimiter
	while (*end == ' ' || *end == '\n')
		end++; // Skip spaces and newlines after delimiter
	new_input_len = strlen(input) - (end - start) + strlen(filename) + 1;
	new_input = (char *)malloc(new_input_len);
	strncpy(new_input, input, start - input);
	new_input[start - input] = '\0';
	strcat(new_input, filename);
	strcat(new_input, end);
	return (new_input);
}

char	*handle_heredoc(char *input)
{
	char	*delimiter;
	char	*heredoc_content;
	char	tmp_filename[] = "/tmp/minishell_heredoc";
	int		fd;
	char	*updated_input;

	delimiter = get_delimiter(input);
	heredoc_content = read_heredoc_input(delimiter);
	if (!heredoc_content)
	{
		free(delimiter);
		return (NULL);
	}
	// Create a temporary file to store the heredoc content
	// Write the heredoc content to the file
	fd = open(tmp_filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	write(fd, heredoc_content, strlen(heredoc_content));
	close(fd);
	// Replace the heredoc content in input with the filename
	updated_input = replace_heredoc_with_filename(input, delimiter,
			tmp_filename);
	free(delimiter);
	free(heredoc_content);
	return (updated_input);
}
