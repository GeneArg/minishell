/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperez-a <bperez-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 15:31:35 by bperez-a          #+#    #+#             */
/*   Updated: 2024/05/27 16:50:20 by bperez-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	join_and_free(char **prompt, char *value)
{
	char	*temp;

	temp = *prompt;
	*prompt = ft_strjoin(*prompt, value);
	free(temp);
}

void	set_colors(char **prompt)
{
	char	*color_user;
	char	*color_path;
	char	*color_reset;
	char	*temp;

	color_user = "\033[1;34m";
	color_path = "\033[0;35m";
	color_reset = "\033[0m";
	temp = ft_strjoin(color_user, *prompt);
	free(*prompt);
	*prompt = temp;
	join_and_free(prompt, color_reset);
	temp = ft_strjoin(color_path, *prompt);
	free(*prompt);
	*prompt = temp;
	join_and_free(prompt, color_reset);
}

char	*get_env_value(t_program *program, char *key)
{
	return (find_env_var_value(program->envp, key));
}

void	get_user_and_path_prompt(char **prompt, char *user, char *pwd,
		char *home)
{
	*prompt = ft_strjoin(user, "@");
	join_and_free(prompt, ":");
	if (!ft_strncmp(pwd, home, ft_strlen(home)))
	{
		join_and_free(prompt, "~");
		join_and_free(prompt, pwd + ft_strlen(home));
	}
	else
	{
		join_and_free(prompt, pwd);
	}
	join_and_free(prompt, " $ ");
}

char	*ft_prompt(t_program *program)
{
	char	*pwd;
	char	*home;
	char	*user;
	char	*prompt;

	pwd = get_env_value(program, "PWD");
	home = get_env_value(program, "HOME");
	user = get_env_value(program, "USER");
	if (!pwd || !home || !user)
		return (ft_strdup("minishell $ "));
	prompt = NULL;
	get_user_and_path_prompt(&prompt, user, pwd, home);
	set_colors(&prompt);
	return (prompt);
}
