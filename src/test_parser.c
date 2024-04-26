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
