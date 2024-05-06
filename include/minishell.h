#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

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
	char **argv;           
		// Arguments vector,including the command itself as the first element
	char *redirect_in;      // Filename for input redirection,or heredoc delimiter
	char *redirect_out;     // Filename for output redirection
	int append;             // Flag for append redirection
	int ret;                // Return value of the command
	char *heredoc_content;  // Heredoc content
	struct s_command *next; // Pointer to the next command in a pipeline, if any
}					t_command;

typedef struct s_program
{
	char			*input;
	t_token			*tokens;
	t_command		*commands;
	char			**envp;
	char			*test;
}					t_program;

// Lexer

char				**splitter(char *input);
t_token				*lex(char *input);

// Parser

t_command			*parse(t_token *tokens);

// Expander

void expand(t_command *commands, char **envp);

// Executer
void				execute(t_program **program);
char				*find_path(char **envp, char *cmd);
char				**get_paths(char **envp);
void				append_str_to_array(char ***array, char *str);
void				free_split(char **split);

// Builtins
void				ft_export(t_program **program, char **argv);

// Quitting & Error Handling

void				free_program(t_program *program, int exit_status);
void				free_program_nonpersistent_values(t_program *program);

// Utils

char *find_env_var_value(char **envp, char *var);

// Test functions

void				test_display(t_program *program);

#endif
