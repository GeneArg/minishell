/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperez-a <bperez-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 14:39:51 by eagranat          #+#    #+#             */
/*   Updated: 2024/06/12 12:41:00 by bperez-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	update_pwd_env_vars(t_program **program, char *pwd)
{
	char	*cwd;
	char	*old_pwd_env_var;
	char	*pwd_env_var;

	cwd = getcwd(NULL, 0);
	old_pwd_env_var = ft_strjoin("OLDPWD=", pwd);
	pwd_env_var = ft_strjoin("PWD=", cwd);
	ft_export(program, (char *[]){"export", old_pwd_env_var, NULL});
	ft_export(program, (char *[]){"export", pwd_env_var, NULL});
	free(cwd);
	free(old_pwd_env_var);
	free(pwd_env_var);
}

char	*get_correct_path(char *arg, char *home, char *oldpwd)
{
	char	*path;

	if (!arg || !ft_strncmp(arg, "~", 2))
		path = home;
	else if (!ft_strncmp(arg, "-", 2))
	{
		path = oldpwd;
		ft_putendl_fd(oldpwd, 1);
	}
	else
		path = arg;
	return (path);
}

int	prepare_cd_path(t_program **program, char **argv, char **path)
{
	char	*home;
	char	*oldpwd;

	home = find_env_var_value((*program)->envp, "HOME");
	if (!home && !argv[1])
	{
		ft_error(program, "cd", "HOME not set", 1);
		return (FAILURE);
	}
	oldpwd = find_env_var_value((*program)->envp, "OLDPWD");
	if (argv[2])
	{
		ft_error(program, "cd", "too many arguments", 1);
		return (FAILURE);
	}
	*path = get_correct_path(argv[1], home, oldpwd);
	return (SUCCESS);
}

int	change_directory_and_update(t_program **program, char *path)
{
	int		ret;
	char	*pwd;

	ret = chdir(path);
	if (ret == -1)
	{
		ft_error(program, "cd", "No such file or directory", 1);
		return (FAILURE);
	}
	pwd = find_env_var_value((*program)->envp, "PWD");
	update_pwd_env_vars(program, pwd);
	return (SUCCESS);
}

int	ft_cd(t_program **program, char **argv)
{
	char	*path;

	if (prepare_cd_path(program, argv, &path) == FAILURE)
		return (FAILURE);
	return (change_directory_and_update(program, path));
}
