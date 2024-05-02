#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "../libft/libft.h"

// Lexer

typedef enum {
    TOKEN_WORD = 1,
    TOKEN_PIPE,
    TOKEN_REDIRECT_IN,       // Input redirection '<'
    TOKEN_REDIRECT_OUT,      // Output redirection '>'
    TOKEN_REDIRECT_APPEND,   // Append redirection '>>'
    TOKEN_HEREDOC,           // Heredoc '<<'
} TokenType;

typedef struct s_token {
    TokenType type;
    char* value;         // The actual text of the token
    struct s_token* next; // Pointer to the next token
} t_token;

char	**splitter(char *input);
t_token *lex(char *input);

// Parser

typedef struct s_command {
    char** argv;             // Arguments vector, including the command itself as the first element
    char* redirect_in;       // Filename for input redirection, or heredoc delimiter
    char* redirect_out;      // Filename for output redirection
    int append;              // Flag for append redirection
    char* heredoc_content;   // Content for heredoc, if applicable
    struct s_command* next;  // Pointer to the next command in a pipeline, if any
} t_command;

t_command *parse(t_token *tokens);


// Expander

// Executer

// Builtins

// Quitting & Error Handling

// Utils

// Test functions

void test_display(char *input);


#endif
