#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include "../libft/libft.h"



// Lexer data structures

typedef enum {
    TOKEN_WORD = 1,              // A generic token for words, could be a command or an argument
    TOKEN_PIPE,              // Pipe '|'
    TOKEN_REDIRECT_IN,       // Input redirection '<'
    TOKEN_REDIRECT_OUT,      // Output redirection '>'
    TOKEN_REDIRECT_APPEND,   // Append redirection '>>'
    TOKEN_HEREDOC,           // Heredoc '<<'
	TOKEN_ENV_VAR,           // Environment variable
} TokenType;

typedef struct s_node
{
	TokenType			type;
	int					file_type;
	char				**args;
	struct s_node		*left;
	struct s_node		*right;
}	t_node;

typedef struct s_token {
    TokenType type;
    char* value;         // The actual text of the token
    struct s_token* next; // Pointer to the next token
} t_token;


// Parser data structures

typedef enum {
    CMD_SIMPLE = 1,           // A simple command
    CMD_PIPELINE,         // A pipeline of commands
    CMD_WITH_REDIRECT_IN, // Command with input redirection
    CMD_WITH_REDIRECT_OUT, // Command with output redirection
    CMD_WITH_REDIRECT_APPEND, // Command with output append redirection
    CMD_WITH_HEREDOC,     // Command with heredoc
} CommandType;

typedef struct s_command {
    CommandType type;        // Type of the command 
    char** argv;             // Arguments vector, including the command itself as the first element ["ls", "-l"]
    char* redirect_in;       // Filename for input redirection, or heredoc delimiter
    char* redirect_out;      // Filename for output redirection
    int append;              // Flag for append redirection
    char* heredoc_content;   // Content for heredoc, if applicable
    struct s_command* next;  // Pointer to the next command in a pipeline, if any
} t_command;

#endif




