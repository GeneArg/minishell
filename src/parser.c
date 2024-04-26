/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eagranat <eagranat@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 09:57:26 by eagranat          #+#    #+#             */
/*   Updated: 2024/04/26 16:23:32 by eagranat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/* create an empty syntax tree
    while there are tokens left {
        if the token is a word {
            add it as a new command to the syntax tree
        } else if the token is a pipe {
            start a new command in the syntax tree
        } else if the token is a redirection {
            add it as a redirection to the current command in the syntax tree
        }
    }
    return the syntax tree
*/

int	count_command_arguments (t_token *current)
{
	int count;

	count = 0;
	while (current && current->type == TOKEN_WORD)
	{
		count++;
		current = current->next;
	}
	return (count);
}



t_node *new_node(TokenType type)
{
	t_node *node;

	node = malloc(sizeof(t_node));
	if (!node)
		return (NULL);
	node->type = type;
	node->args = NULL;
	node->left = NULL;
	node->right = NULL;
	return (node);
}

t_node *process_command(t_token **tokens)
{
	t_node *command_node;
	int arg_count;
	int i;
	t_token *temp;

	command_node = new_node(TOKEN_WORD);
	arg_count = count_command_arguments(*tokens);
	command_node->args = malloc(sizeof(char *) * (arg_count + 1));
	if (!command_node->args)
		return (NULL);
	i = 0;
	while (i < arg_count)
	{
		command_node->args[i] = ft_strdup((*tokens)->value);
		temp = *tokens;
		*tokens = (*tokens)->next;
		free(temp->value);
		free(temp);
		i++;
	}
	command_node->args[arg_count] = NULL;
	return (command_node);
}

t_node *create_file_node(t_token *token)
{
	t_node *node;

	node = malloc(sizeof(t_node));
	if (!node)
		return (NULL);
	node->type = token->type;
	node->args = malloc(sizeof(char *) * 2);
	if (!node->args)
	{
		free(node);
		return (NULL);
	}
	node->args[0] = token->value;
	node->args[1] = NULL;
	node->left = NULL;
	node->right = NULL;
	free(token);
	return (node);
}

t_node *parse_command(t_token **tokens)
{
	t_token *current;
	t_node *redirect_node;
	t_token *next_token;

	if (!*tokens)
		return (NULL);
	current = *tokens;
	if ((*tokens)->type >= TOKEN_REDIRECT_IN && (*tokens)->type <= TOKEN_HEREDOC)
	{
		redirect_node = new_node((*tokens)->type);
		*tokens = (*tokens)->next->next;
		redirect_node->left = parse_command(tokens);
		redirect_node->right = create_file_node(current->next);
		free(current->value);
		free(current);
		return (redirect_node);
	}
	while (*tokens && (*tokens)->next)
	{
		next_token = (*tokens)->next;
		if ((*tokens)->next->type >= TOKEN_REDIRECT_IN && (*tokens)->next->type <= TOKEN_HEREDOC)
		{
			redirect_node = new_node((*tokens)->next->type);
			(*tokens)->next = next_token->next->next;
			redirect_node->left = parse_command(&current);
			redirect_node->right = create_file_node(next_token->next);
			free(next_token->value);
			free(next_token);
			return (redirect_node);
		}
		*tokens = next_token;
	}
	return (process_command(&current));
		
}

t_node *parse_pipeline(t_token **tokens)
{
	t_token *temp;
	t_token *next_token = NULL;
	t_node *pipeline_node;
	
	temp = *tokens;
	while (*tokens && (*tokens)->next)
	{
		if ((*tokens)->next->type == TOKEN_PIPE)
		{
			pipeline_node = new_node((*tokens)->next->type);
			(*tokens)->next = NULL;
			pipeline_node->left = parse_command(&temp);
			pipeline_node->right = parse_pipeline(&(next_token->next));
			free(next_token->value);
			free(next_token);
			return (pipeline_node);
		}
		*tokens = next_token;
	}
	return (parse_command(&temp));
}

t_node *parser(t_token **tokens) 
{	
    if (!tokens || !*tokens)
		return (NULL);
	return (parse_pipeline(tokens));
}



/*==============================================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/minishell.h"

void test_count_command_arguments()
{
    t_token *token1 = malloc(sizeof(t_token));
    t_token *token2 = malloc(sizeof(t_token));
    t_token *token3 = malloc(sizeof(t_token));
    t_token *token4 = malloc(sizeof(t_token));

    token1->type = TOKEN_WORD;
    token2->type = TOKEN_WORD;
    token3->type = TOKEN_WORD;
    token4->type = TOKEN_PIPE;

    token1->next = token2;
    token2->next = token3;
    token3->next = token4;
    token4->next = NULL;

    int count = count_command_arguments(token1);

    printf("Number of command arguments: %d\n", count);

    free(token1);
    free(token2);
    free(token3);
    free(token4);
}

void test_process_command()
{
    t_token *token1 = malloc(sizeof(t_token));
    t_token *token2 = malloc(sizeof(t_token));
    t_token *token3 = malloc(sizeof(t_token));

    token1->type = TOKEN_WORD;
    token2->type = TOKEN_WORD;
    token3->type = TOKEN_PIPE;

    token1->value = strdup("ls");
    token2->value = strdup("-l");
    token3->value = strdup("|");

    token1->next = token2;
    token2->next = token3;
    token3->next = NULL;

    t_node *command_node = process_command(&token1);

    printf("Command node type: %d\n", command_node->type);
    printf("Command node arguments: ");
    for (int i = 0; command_node->args[i] != NULL; i++)
    {
        printf("%s ", command_node->args[i]);
    }
    printf("\n");

    free(command_node->args);
    free(command_node);
    free(token1->value);
    free(token2->value);
    free(token3->value);
    free(token1);
    free(token2);
    free(token3);
}

void test_create_file_node()
{
    t_token *token = malloc(sizeof(t_token));
    token->type = TOKEN_REDIRECT_OUT;
    token->value = strdup("output.txt");

    t_node *file_node = create_file_node(token);

    printf("File node type: %d\n", file_node->type);
    printf("File node arguments: ");
    for (int i = 0; file_node->args[i] != NULL; i++)
    {
        printf("%s ", file_node->args[i]);
    }
    printf("\n");

    free(file_node->args);
    free(file_node);
    free(token->value);
    free(token);
}

void test_parse_command()
{
    t_token *token1 = malloc(sizeof(t_token));
    t_token *token2 = malloc(sizeof(t_token));
    t_token *token3 = malloc(sizeof(t_token));
    t_token *token4 = malloc(sizeof(t_token));
    t_token *token5 = malloc(sizeof(t_token));

    token1->type = TOKEN_WORD;
    token2->type = TOKEN_REDIRECT_OUT;
    token3->type = TOKEN_WORD;
    token4->type = TOKEN_WORD;
    token5->type = TOKEN_PIPE;

    token1->value = strdup("echo");
    token2->value = strdup("hello.txt");
    token3->value = strdup("Hello");
    token4->value = strdup("World");
    token5->value = strdup("|");

    token1->next = token2;
    token2->next = token3;
    token3->next = token4;
    token4->next = token5;
    token5->next = NULL;

    t_node *command_node = parse_command(&token1);

    printf("Command node type: %d\n", command_node->type);
    printf("Command node arguments: ");
    for (int i = 0; command_node->args[i] != NULL; i++)
    {
        printf("%s ", command_node->args[i]);
    }
    printf("\n");

    free(command_node->args);
    free(command_node);
    free(token1->value);
    free(token2->value);
    free(token3->value);
    free(token4->value);
    free(token5->value);
    free(token1);
    free(token2);
    free(token3);
    free(token4);
    free(token5);
}

void test_parse_pipeline()
{
    t_token *token1 = malloc(sizeof(t_token));
    t_token *token2 = malloc(sizeof(t_token));
    t_token *token3 = malloc(sizeof(t_token));
    t_token *token4 = malloc(sizeof(t_token));
    t_token *token5 = malloc(sizeof(t_token));
    t_token *token6 = malloc(sizeof(t_token));

    token1->type = TOKEN_WORD;
    token2->type = TOKEN_REDIRECT_OUT;
    token3->type = TOKEN_WORD;
    token4->type = TOKEN_PIPE;
    token5->type = TOKEN_WORD;
    token6->type = TOKEN_WORD;

    token1->value = strdup("echo");
    token2->value = strdup("hello.txt");
    token3->value = strdup("Hello");
    token4->value = strdup("|");
    token5->value = strdup("grep");
    token6->value = strdup("World");

    token1->next = token2;
    token2->next = token3;
    token3->next = token4;
    token4->next = token5;
    token5->next = token6;
    token6->next = NULL;

    t_node *pipeline_node = parse_pipeline(&token1);

    printf("Pipeline node type: %d\n", pipeline_node->type);
    printf("Pipeline node left command type: %d\n", pipeline_node->left->type);
    printf("Pipeline node left command arguments: ");
    for (int i = 0; pipeline_node->left->args[i] != NULL; i++)
    {
        printf("%s ", pipeline_node->left->args[i]);
    }
    printf("\n");
    printf("Pipeline node right command type: %d\n", pipeline_node->right->type);
    printf("Pipeline node right command arguments: ");
    for (int i = 0; pipeline_node->right->args[i] != NULL; i++)
    {
        printf("%s ", pipeline_node->right->args[i]);
    }
    printf("\n");

    free(pipeline_node->left->args);
    free(pipeline_node->left);
    free(pipeline_node->right->args);
    free(pipeline_node->right);
    free(pipeline_node);
    free(token1->value);
    free(token2->value);
    free(token3->value);
    free(token4->value);
    free(token5->value);
    free(token6->value);
    free(token1);
    free(token2);
    free(token3);
    free(token4);
    free(token5);
    free(token6);
}

void test_parser()
{
    t_token *token1 = malloc(sizeof(t_token));
    t_token *token2 = malloc(sizeof(t_token));
    t_token *token3 = malloc(sizeof(t_token));
    t_token *token4 = malloc(sizeof(t_token));
    t_token *token5 = malloc(sizeof(t_token));
    t_token *token6 = malloc(sizeof(t_token));

    token1->type = TOKEN_WORD;
    token2->type = TOKEN_REDIRECT_OUT;
    token3->type = TOKEN_WORD;
    token4->type = TOKEN_PIPE;
    token5->type = TOKEN_WORD;
    token6->type = TOKEN_WORD;

    token1->value = strdup("echo");
    token2->value = strdup("hello.txt");
    token3->value = strdup("Hello");
    token4->value = strdup("|");
    token5->value = strdup("grep");
    token6->value = strdup("World");

    token1->next = token2;
    token2->next = token3;
    token3->next = token4;
    token4->next = token5;
    token5->next = token6;
    token6->next = NULL;

    t_node *root_node = parser(&token1);

    printf("Root node type: %d\n", root_node->type);
    printf("Root node left command type: %d\n", root_node->left->type);
    printf("Root node left command arguments: ");
    for (int i = 0; root_node->left->args[i] != NULL; i++)
    {
        printf("%s ", root_node->left->args[i]);
    }
    printf("\n");
    printf("Root node right command type: %d\n", root_node->right->type);
    printf("Root node right command arguments: ");
    for (int i = 0; root_node->right->args[i] != NULL; i++)
    {
        printf("%s ", root_node->right->args[i]);
    }
    printf("\n");

    free(root_node->left->args);
    free(root_node->left);
    free(root_node->right->args);
    free(root_node->right);
    free(root_node);
    free(token1->value);
    free(token2->value);
    free(token3->value);
    free(token4->value);
    free(token5->value);
    free(token6->value);
    free(token1);
    free(token2);
    free(token3);
    free(token4);
    free(token5);
    free(token6);
}

int main()
{
    test_count_command_arguments();
    test_process_command();
    test_create_file_node();
    test_parse_command();
    test_parse_pipeline();
    test_parser();

    return 0;
}
