/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eagranat <eagranat@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 11:24:35 by bperez-a          #+#    #+#             */
/*   Updated: 2024/04/26 16:24:50 by eagranat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../include/minishell.h"


char **splitter(char *input)
{
	bool in_double_quotes;
	bool in_simple_quotes;

	in_double_quotes = false;
	in_simple_quotes = false;

	char **ret = malloc(sizeof(char *) * 100);
	int i = 0;
	int word_len;

	char *current_word = malloc(sizeof(char) * 100);
	current_word[0] = '\0';
	int j = 0;
	
	while (*input)
	{
		if (*input == '"' && !in_simple_quotes)
		{
			if (current_word)
			{
				ret[i] = ft_strdup(ft_strtrim(current_word, " "));
				i++;
				free(current_word);
				current_word = malloc(sizeof(char) * 100);
				current_word[0] = '\0';
				j = 0;
			}
			in_double_quotes = true;
			word_len = ft_strchr(input + 1, '"') - input;
			ret[i] = ft_substr(input, 0, word_len + 1);
			input += word_len + 1;
			i++;
			in_double_quotes = false;
		}
		else if (*input == '\'' && !in_double_quotes)
		{
			if (current_word)
			{
				ret[i] = ft_strdup(ft_strtrim(current_word, " "));
				i++;
				free(current_word);
				current_word = malloc(sizeof(char) * 100);
				current_word[0] = '\0';
				j = 0;
			}
			in_simple_quotes = true;
			word_len = ft_strchr(input + 1, '\'') - input;
			ret[i] = ft_substr(input, 0, word_len + 1);
			input += word_len + 1;
			i++;
			in_simple_quotes = false;
		}
		else if (*input == '|')
		{
			if (current_word)
			{
				ret[i] = ft_strdup(ft_strtrim(current_word, " "));
				i++;
				free(current_word);
				current_word = malloc(sizeof(char) * 100);
				current_word[0] = '\0';
				j = 0;
			}
			ret[i] = ft_strdup("|");
			i++;
			input++;
		}
		else if (*input == '>')
		{
			if (current_word)
			{
				ret[i] = ft_strdup(ft_strtrim(current_word, " "));
				i++;
				free(current_word);
				current_word = malloc(sizeof(char) * 100);
				current_word[0] = '\0';
				j = 0;
			}
			if (*(input + 1) == '>')
			{
				ret[i] = ft_strdup(">>");
				i++;
				input+=2;
			}
			else
			{
				ret[i] = ft_strdup(">");
				i++;
				input++;
			}
		}
		else if (*input == '<')
		{
			if (current_word)
			{
				ret[i] = ft_strdup(ft_strtrim(current_word, " "));
				i++;
				free(current_word);
				current_word = malloc(sizeof(char) * 100);
				current_word[0] = '\0';
				j = 0;
			}
			if (*(input + 1) == '<')
			{
				ret[i] = ft_strdup("<<");
				i++;
				input+=2;
			}
			else
			{
				ret[i] = ft_strdup("<");
				i++;
				input++;
			}
		}
		else if (*input == ' ')
		{
			if (current_word)
			{
				ret[i] = ft_strdup(ft_strtrim(current_word, " "));
				i++;
				free(current_word);
				current_word = malloc(sizeof(char) * 100);
				current_word[0] = '\0';
				j = 0;
			}
		}
		current_word[j++] = *input;
		current_word[j] = '\0';
		input++;
	}
	if (current_word)
	{
		ret[i] = ft_strdup(ft_strtrim(current_word, " "));
		i++;
		free(current_word);
	}
	ret[i] = NULL;
	return (ret);
}

t_token *populate_list(char **split)
{
	t_token *head = malloc(sizeof(t_token));
	t_token *current = head;
	int i = 0;
	
	while(split[i])
	{	
		if (ft_strlen(split[i]) == 0)
			i++;
		else if (split[i][0] == '"' || split[i][0] == '\'')
		{
			current->type = TOKEN_WORD;
			current->value = split[i];
			current->next = malloc(sizeof(t_token));
			current = current->next;
			i++;
		}
		else if (split[i][0] == '|')
		{
			current->type = TOKEN_PIPE;
			current->value = split[i];
			current->next = malloc(sizeof(t_token));
			current = current->next;
			i++;
		}
		else if (split[i][0] == '<' && split[i][1] == '<')
		{
			current->type = TOKEN_HEREDOC;
			current->value = split[i];
			current->next = malloc(sizeof(t_token));
			current = current->next;
			i++;
		}
		else if (split[i][0] == '<')
		{
			current->type = TOKEN_REDIRECT_IN;
			current->value = split[i];
			current->next = malloc(sizeof(t_token));
			current = current->next;
			i++;
		}
		else if (split[i][0] == '>' && split[i][1] == '>')
		{
			current->type = TOKEN_REDIRECT_APPEND;
			current->value = split[i];
			current->next = malloc(sizeof(t_token));
			current = current->next;
			i++;
		}
		else if (split[i][0] == '>')
		{
			current->type = TOKEN_REDIRECT_OUT;
			current->value = split[i];
			current->next = malloc(sizeof(t_token));
			current = current->next;
			i++;
		}
		else
		{
			current->type = TOKEN_WORD;
			current->value = split[i];
			current->next = malloc(sizeof(t_token));
			current = current->next;
			i++;
		}
	}
	current->next = NULL;
	return (head);
}


// int main(void)
// {
// 	char *test = "sort \"list.txt\" | uniq -c | sort -nr > 'result file.txt'";
// 	char **split = splitter(test);
// 	int i = 0;
// 	printf("Split:\n");
// 	while (split[i])
// 	{
// 		printf("%s\n", split[i]);
// 		i++;
// 	}
// 	t_token *token = populate_list(split);
// 	t_token *current = token;
// 	printf("Token list:\n");
// 	while (current->value)
// 	{
// 		printf("Type: %d, Value: %s\n", current->type, current->value);
// 		current = current->next;
// 	}
// }


