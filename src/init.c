/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperez-a <bperez-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 15:42:17 by bperez-a          #+#    #+#             */
/*   Updated: 2024/05/27 10:08:22 by bperez-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	init_env(t_program **program, char **envp)
{
	(*program)->envp = ft_copy_array(envp);
}

void	init_pwd(t_program **program)
{
	char	*cwd;
	char	*pwd;
	char	*oldpwd;

	cwd = getcwd(NULL, 0);
	pwd = ft_strjoin("PWD=", cwd);
	oldpwd = ft_strjoin("OLDPWD=", cwd);
	ft_export(program, (char *[]){"export", pwd, NULL});
	ft_export(program, (char *[]){"export", oldpwd, NULL});
	free(cwd);
	free(pwd);
	free(oldpwd);
}

void	increase_shlvl(t_program **program)
{
	char	*shlvl;
	int		shlvl_int;
	char	*new_shlvl;
	char	*shlvl_var;

	shlvl = find_env_var_value((*program)->envp, "SHLVL");
	if (!shlvl)
	{
		ft_export(program, (char *[]){"export", ft_strdup("SHLVL=1"), NULL});
		return ;
	}
	shlvl_int = ft_atoi(shlvl);
	new_shlvl = ft_itoa(shlvl_int + 1);
	shlvl_var = ft_strjoin("SHLVL=", new_shlvl);
	ft_export(program, (char *[]){"export", shlvl_var, NULL});
	free(shlvl_var);
	free(new_shlvl);
}

t_program	*init_program(char **envp)
{
	t_program	*program;

	program = malloc(sizeof(t_program));
	program->input = NULL;
	program->tokens = NULL;
	program->commands = NULL;
	program->envp = ft_copy_array(envp);
	ft_export(&program, (char *[]){"export", "?=0", NULL});
	increase_shlvl(&program);
	init_pwd(&program);
	return (program);
}
