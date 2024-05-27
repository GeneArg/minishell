/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperez-a <bperez-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 10:11:42 by bperez-a          #+#    #+#             */
/*   Updated: 2024/05/27 16:54:30 by bperez-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>

// Enums

typedef enum e_redirection_type
{
	REDIRECT_IN = 1,
	REDIRECT_OUT,
}							t_redirection_type;

typedef enum e_token_type
{
	TOKEN_WORD = 1,
	TOKEN_PIPE,
	TOKEN_REDIRECT_IN,     // Input redirection '<'
	TOKEN_REDIRECT_OUT,    // Output redirection '>'
	TOKEN_REDIRECT_APPEND, // Append redirection '>>'
}							t_token_type;

typedef enum e_shell_exit_code
{
	SUCCESS = 0,             // Successful operation
	FAILURE = 1,             // General failure
	COMMAND_NOT_FOUND = 127, // Command not found
	CANNOT_EXECUTE = 126,    // Command invoked cannot execute
	INVALID_ARGS = 2,        // Invalid arguments provided
	EXEC_FAIL = 3,           // Execution failure
	ENV_FAIL = 4,            // Environment handling failure
	PERMISSION_DENIED = 5,   // Permission denied error
	MEMORY_ERROR = 6,        // Memory allocation failed
	PIPE_FAIL = 7,           // Pipe creation failed
	FORK_FAIL = 8,           // Fork failed
	OTHER_ERROR = 99         // Other unspecified errors
}							t_shell_exit_code;

// Structures

typedef struct s_split
{
	char					*current_word;
	int						i;
	int						j;
	bool					in_double_quotes;
	bool					in_simple_quotes;
}							t_split;

typedef struct s_token
{
	t_token_type			type;
	char					*value;
	struct s_token			*next;
}							t_token;

typedef struct s_redirection
{
	char					*file;
	t_redirection_type		type;
	struct s_redirection	*next;
}							t_redirection;

typedef struct s_command
{
	char					**argv;
	t_redirection			*redirects;
	int						append;
	struct s_command		*next;
	int						flag_error;
	int						ret;
}							t_command;

typedef struct s_program
{
	char					*input;
	t_token					*tokens;
	t_command				*commands;
	char					**envp;
}							t_program;

// Lexer

char						**splitter(char *input);
void						process_input_string(char *input, char **ret,
								t_split *split_vars);
t_split						*initialize_split_vars(void);
void						finalize_current_word(char **ret,
								t_split *split_vars);
void						handle_space(char **ret, t_split *split_vars);
void						handle_special_chars(char **input, char **ret,
								t_split *split_vars);
void						handle_quotes(char **input, t_split *split_vars,
								char quote);
void						add_current_word_to_ret(char **ret,
								t_split *split_vars);
t_token						*lex(char *input);
t_token						*populate_list(char **split);
void						append_token(t_token **current, t_token_type type,
								char *value);
t_token_type				determine_type(char *token);

// Parser

int							check_syntax(t_token *token, t_program **program);
t_command					*parse(t_token *tokens);

// Expander

void						expand(t_command *commands, char **envp);

// Executor

void						execute(t_program **program);
char						*find_path(char **envp, char *cmd);
char						**get_paths(char **envp);
void						append_str_to_array(char ***array, char *str);
void						free_split(char **split);
void						check_access(char *cmd_path, t_command *cmd);
pid_t						execute_in_child(t_command *cmd,
								t_program **program, int in_fd, int out_fd,
								char **env_copy);

// Builtins

int							ft_export(t_program **program, char **argv);
void						add_env(char ***envp, char *new_env);
int							ft_echo(char **argv);
int							ft_cd(t_program **program, char **argv);
int							ft_unset(t_program **program, char **argv);
void						ft_exit(t_program **program,
								t_command *current_command);
int							ft_pwd(void);
int							ft_env(t_program **program);

// Initializations

t_program					*init_program(char **envp);
void						init_env(t_program **program, char **envp);
void						init_pwd(t_program **program);
void						increase_shlvl(t_program **program);

// Pretty

char						*ft_prompt(t_program *program);
// static void				print_welcome_msg(void);

// Quitting & Freeing

void						free_and_exit(t_program *program, int exit_status);
void						free_program(t_program *program);
void						free_commands(t_command *commands);
void						free_tokens(t_token *tokens);
void						free_redirects(t_redirection *redirects);

// Errors

void						ft_error(t_program **program, char *cmd,
								char *error, int exit_status);

// Utils

char						*find_env_var_value(char **envp, char *var);
int							find_env_var(char **envp, char *var);
void						set_env_var(t_program **program, char *key,
								char *value);
char						**ft_dup_array(char **array);
void						ft_free_array(char **array);

// Signals

void						handle_sigint_blocking(int sig);
void						handle_sigint_non_blocking(int sig);
void						handle_sigquit(int sig);

// Heredoc

char						*handle_heredoc(char *input);
bool						is_heredoc(char *input);

#endif
