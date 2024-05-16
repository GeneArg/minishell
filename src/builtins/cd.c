/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperez-a <bperez-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 14:39:51 by eagranat          #+#    #+#             */
/*   Updated: 2024/05/16 13:36:24 by bperez-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_cd(t_program **program, char **argv)
{
	char *home;
	char *oldpwd;
	char *pwd;
	char *path;
	int ret;

	home = find_env_var_value((*program)->envp, "HOME");
	oldpwd = find_env_var_value((*program)->envp, "OLDPWD");
	pwd = find_env_var_value((*program)->envp, "PWD");
	if (ft_array_len(argv) > 2)
	{
		ft_error(program, "cd", "too many arguments", 1);		
		return (FAILURE);
	}
	if (!argv[1] || !ft_strncmp(argv[1], "~", 2))
		path = home;
	else if (!ft_strncmp(argv[1], "-", 2))
		path = oldpwd;
	else
		path = argv[1];
	ret = chdir(path);
	if (ret == -1)
	{
		ft_error(program, "cd", "No such file or directory", 1);
		return (FAILURE);
	}
	char *cwd = getcwd(NULL, 0);
	char *old_pwd_env_var = ft_strjoin("OLDPWD=", pwd);
	char *pwd_env_var = ft_strjoin("PWD=", cwd);
	ft_export(program, (char *[]){"export", old_pwd_env_var, NULL});
	ft_export(program, (char *[]){"export", pwd_env_var, NULL});

	free(cwd);
	free(old_pwd_env_var);
	free(pwd_env_var);
	return (SUCCESS);
}
