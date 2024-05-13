/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperez-a <bperez-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 12:56:22 by bperez-a          #+#    #+#             */
/*   Updated: 2024/05/13 11:22:20 by bperez-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"



bool is_enclosed_in_single_quotes(char *arg) {
    int len = strlen(arg);
    return (len >= 2 && arg[0] == '\'' && arg[len-1] == '\'');
}

void trim_quotes(char **arg) {
    char *input = *arg;
    int length = strlen(input);
    char *output = malloc(length - 1);  // Allocate slightly less assuming we'll remove at least two characters (quotes)
    if (!output) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }

    int i = 0, j = 0;
    bool inside_quotes = false;
    char current_quote = 0;

    while (input[i] != '\0') {
        if ((input[i] == '\'' || input[i] == '\"') && (!inside_quotes || current_quote == input[i])) {
            if (!inside_quotes) {
                inside_quotes = true;  // Opening quote found
                current_quote = input[i];  // Set the type of quote we're inside
            } else {
                inside_quotes = false;  // Closing quote found
                current_quote = 0;  // Reset the quote type
            }
        } else if (!inside_quotes || (inside_quotes && current_quote != input[i])) {
            // Copy characters only if not inside a different type of quote
            output[j++] = input[i];
        }
        i++;
    }

    output[j] = '\0';  // Null-terminate the output string
    free(*arg);  // Free the original argument string
    *arg = output;  // Replace original with trimmed output
}


void replace_env_variables(char **arg, char **env) {
    char *input = *arg;
    bool in_single_quotes = false;
    char *output = malloc(strlen(input) + 1024); // Larger buffer for expanded variables
    if (!output) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }

    char *cur = input;
    char *out = output;
    while (*cur) {
        if (*cur == '\'' && !in_single_quotes) {
            in_single_quotes = true;
            *out++ = *cur++; // Copy single quote to output
        } else if (*cur == '\'' && in_single_quotes) {
            in_single_quotes = false;
            *out++ = *cur++; // Copy closing single quote to output
        } else if (!in_single_quotes && *cur == '$' &&
            (*(cur + 1) == '?' || ft_isalnum((unsigned char)*(cur + 1)) || *(cur + 1) == '_')) {
            // Process environment variable replacement
            char *end = cur + 1;
            if (*end != '?') {
                while (ft_isalnum((unsigned char)*end) || *end == '_' || *end == '?') end++;
            } else {
                end++;
            }
            size_t var_len = end - (cur + 1);
            char var_name[var_len + 1];
            strncpy(var_name, cur + 1, var_len);
            var_name[var_len] = '\0';

            char *value = NULL;
            for (int i = 0; env[i] != NULL; i++) {
                if (strncmp(env[i], var_name, var_len) == 0 && env[i][var_len] == '=') {
                    value = env[i] + var_len + 1;
                    break;
                }
            }
            if (value) {
                strcpy(out, value);
                out += strlen(value);
            }
            cur = end;
        } else {
            *out++ = *cur++;
        }
    }
    *out = '\0';
    free(*arg);
    *arg = output;
}

void remove_empty_args(char ***argv) {
	int count = 0;
	for (int i = 0; (*argv)[i] != NULL; i++) {
		if (strlen((*argv)[i]) > 0) {
			(*argv)[count++] = (*argv)[i];
		}
	}
	(*argv)[count] = NULL;
}

void expand(t_command *commands, char **envp) {
    for (t_command *current = commands; current != NULL; current = current->next) {
        for (int i = 0; current->argv[i] != NULL; i++) {
            if (is_enclosed_in_single_quotes(current->argv[i])) {
                // Just trim quotes for single-quoted arguments, no expansion
                trim_quotes(&(current->argv[i]));
            } else {
                // For all other cases, trim quotes and then expand variables
                trim_quotes(&(current->argv[i]));
                replace_env_variables(&(current->argv[i]), envp);
            }
        }
		remove_empty_args(&(current->argv));
    }
}
