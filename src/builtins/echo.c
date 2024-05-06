/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eagranat <eagranat@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 15:52:31 by eagranat          #+#    #+#             */
/*   Updated: 2024/05/06 16:46:56 by eagranat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_echo(char **argv)
{
	int	i;
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