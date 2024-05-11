/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperez-a <bperez-a@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 14:39:51 by eagranat          #+#    #+#             */
/*   Updated: 2024/05/11 08:40:14 by bperez-a         ###   ########.fr       */
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
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		return (1);

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
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": No such file or directory\n", 2);

		return (1);
	}
	ft_export(program, (char *[]){"export", ft_strjoin("OLDPWD=", pwd), NULL});
	ft_export(program, (char *[]){"export", ft_strjoin("PWD=", getcwd(NULL, 0)), NULL});
	return (0);
}
