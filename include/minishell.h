#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

// Structures

typedef enum
{
	TOKEN_WORD = 1,
	TOKEN_PIPE,
	TOKEN_REDIRECT_IN,     // Input redirection '<'
	TOKEN_REDIRECT_OUT,    // Output redirection '>'
	TOKEN_REDIRECT_APPEND, // Append redirection '>>'
	TOKEN_HEREDOC,         // Heredoc '<<'
}					TokenType;

typedef struct s_token
{
	TokenType		type;
	char			*value;
	struct s_token	*next;
}					t_token;

typedef struct s_command
{
	char **argv;			// Arguments vector,including the command itself as the first element
	char *redirect_in;      // Filename for input redirection, or heredoc delimiter
	char *redirect_out;     // Filename for output redirection
	int append;             // Flag for append redirection
	char *heredoc_content;  // Heredoc content
	struct s_command *next; // Pointer to the next command in a pipeline, if any
}					t_command;

typedef struct s_program
{
	char			*input;
	t_token			*tokens;
	t_command		*commands;
}					t_program;

// Lexer

char				**splitter(char *input);
t_token				*lex(char *input);

// Parser

t_command			*parse(t_token *tokens);

// Expander

// Executer

// Builtins

// Quitting & Error Handling

void				free_program(t_program *program, int exit_status);
void				free_program_nonpersistent_values(t_program *program);

// Utils

// Test functions

void				test_display(t_program *program);

#endif
