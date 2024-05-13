/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperez-a <bperez-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 15:42:17 by bperez-a          #+#    #+#             */
/*   Updated: 2024/05/13 15:51:07 by bperez-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_program	*init_program(void)
{
	t_program	*program;

	program = malloc(sizeof(t_program));
	program->input = NULL;
	program->tokens = NULL;
	program->commands = NULL;
	return (program);
}

void	init_env(t_program **program, char **envp)
{
	(*program)->envp = ft_copy_array(envp);
}

void	init_pwd(t_program **program)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	ft_export(program, (char *[]){"export", ft_strjoin("PWD=", pwd), NULL});
	ft_export(program, (char *[]){"export", ft_strjoin("OLDPWD=", pwd), NULL});
	free(pwd);
}

void increase_shlvl(t_program **program)
{
	char	*shlvl;
	int		shlvl_int;
	char	*new_shlvl;

	shlvl = find_env_var_value((*program)->envp, "SHLVL");
	if (!shlvl)
	{
		ft_export(program, (char *[]){"export", ft_strdup("SHLVL=1"), NULL});
		return ;
	}
	shlvl_int = ft_atoi(shlvl);
	new_shlvl = ft_itoa(shlvl_int + 1);
	ft_export(program, (char *[]){"export", ft_strjoin("SHLVL=", new_shlvl),
		NULL});
	free(new_shlvl);
}