/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eagranat <eagranat@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 12:56:22 by bperez-a          #+#    #+#             */
/*   Updated: 2024/05/07 23:25:08 by eagranat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"



bool is_enclosed_in_single_quotes(char *arg)
{
	return (arg[0] == '\'' && arg[ft_strlen(arg) - 1] == '\'');
}

bool is_enclosed_in_double_quotes(char *arg)
{
	return (arg[0] == '"' && arg[ft_strlen(arg) - 1] == '"');
}

void	trim_quotes(char **arg)
{
	char	*trimmed;
	if (is_enclosed_in_single_quotes(*arg))
		trimmed = ft_strtrim(*arg, "'");
	else if (is_enclosed_in_double_quotes(*arg))
		trimmed = ft_strtrim(*arg, "\"");
	else
		return;
	free(*arg);
	*arg = trimmed;
}

void replace_env_variables(char **arg, char **env)
{
    char *input = *arg;
    char *output = malloc(strlen(input) + 1);
    char *cur = input;
	char *out = output;

    while (*cur) {
        if (*cur == '$' && (*(cur + 1) == '?' ||ft_isalnum((unsigned char)*(cur + 1)))){
            char *end = cur + 1;
			if (*end != '?')
			{
            	while (ft_isalnum((unsigned char)*end) || *end == '_' || *end == '?')
                	end++;  // Find the end of the variable name
			}
			else
				end++;
            size_t var_len = end - (cur + 1);
            char var_name[var_len + 1];
            strncpy(var_name, cur + 1, var_len);
            var_name[var_len] = '\0';  // Null-terminate the variable name

            char *value = NULL;
            for (int i = 0; env[i] != NULL; i++) {
                if (ft_strncmp(env[i], var_name, var_len) == 0 && env[i][var_len] == '=') {
                    value = env[i] + var_len + 1;
                    break;
                }
            }
            if (value) {
                strcpy(out, value);
                out += ft_strlen(value);
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

void expand(t_command *commands, char **envp)
{
	for (t_command *current = commands; current != NULL; current = current->next)
	{
		for (int i = 0; current->argv[i] != NULL; i++)
		{
			if (!is_enclosed_in_single_quotes(current->argv[i]))
				replace_env_variables(&(current->argv[i]), envp);
			trim_quotes(&(current->argv[i]));
		}
	}
}
