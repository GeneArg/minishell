/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperez-a <bperez-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 15:52:31 by eagranat          #+#    #+#             */
/*   Updated: 2024/05/13 10:36:00 by bperez-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_echo(char **argv)
{
	int		i;
	bool	n_option;

	n_option = false;
	i = 1;
	while (argv[i] && !ft_strncmp(argv[i], "-n", 2) && ft_strlen(argv[i]) == 2)
	{
		n_option = true;
		i++;
	}
	if (!argv[i])
	{
		if (!n_option)
			ft_putstr_fd("\n", 1);
	}
	while (argv[i])
	{
		ft_putstr_fd(argv[i], 1);
		if (argv[i + 1])
			ft_putstr_fd(" ", 1);
		else if (!argv[i + 1] && !n_option)
			ft_putstr_fd("\n", 1);
		i++;
	}
	return (0);
}
