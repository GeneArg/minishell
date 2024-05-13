#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <errno.h>
# include <readline/history.h>
# include <readline/readline.h>
#include <sys/stat.h>


// Structures

typedef struct s_redirection
{
	char *file;
	struct s_redirection *next;
}					t_redirection;

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
	int ret;                // Return value of the command
	char *heredoc_content;  // Heredoc content
	struct s_command *next; // Pointer to the next command in a pipeline, if any
	t_redirection *redirect_in;	// Input redirection
	char *redirect_out;     // Filename for output redirection
	int append;					// 1 if output redirection should append, 0 otherwise
	int flag_error;				// 1 if there was an error in the command
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
int					ft_export(t_program **program, char **argv);
void 				add_env(char ***envp, char *new_env);
int					ft_echo(char **argv);
int					ft_cd(t_program **program, char **argv);
int 				ft_unset(t_program **program, char **argv);
void				ft_exit(t_program **program, t_command *current_command);
int	ft_pwd(void);
int ft_env(t_program **program);


// Quitting & Error Handling

void				free_program(t_program *program, int exit_status);
void				free_program_nonpersistent_values(t_program *program);

// Utils

char 				*find_env_var_value(char **envp, char *var);
int					find_env_var(char **envp, char *var);


// Test functions


#endif
