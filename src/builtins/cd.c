/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eagranat <eagranat@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 14:39:51 by eagranat          #+#    #+#             */
/*   Updated: 2024/05/25 23:56:24 by eagranat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_cd(t_program **program, char **argv)
{
	char	*home;
	char	*oldpwd;
	char	*pwd;
	char	*path;
	int		ret;
	char	*cwd;
	char	*old_pwd_env_var;
	char	*pwd_env_var;

	home = find_env_var_value((*program)->envp, "HOME");
	oldpwd = find_env_var_value((*program)->envp, "OLDPWD");
	pwd = find_env_var_value((*program)->envp, "PWD");
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
	cwd = getcwd(NULL, 0);
	old_pwd_env_var = ft_strjoin("OLDPWD=", pwd);
	pwd_env_var = ft_strjoin("PWD=", cwd);
	ft_export(program, (char *[]){"export", old_pwd_env_var, NULL});
	ft_export(program, (char *[]){"export", pwd_env_var, NULL});
	free(cwd);
	free(old_pwd_env_var);
	free(pwd_env_var);
	return (SUCCESS);
}
