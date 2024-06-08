/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eagranat <eagranat@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 15:52:31 by eagranat          #+#    #+#             */
/*   Updated: 2024/06/08 12:11:37 by eagranat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

bool	is_all_n_flags(char *arg)
{
	int	j;

	j = 2;
	if (arg[0] != '-' || arg[1] != 'n')
		return (false);
	while (arg[j] != '\0')
	{
		if (arg[j] != 'n')
			return (false);
		j++;
	}
	return (true);
}

int	skip_n_flags(char **argv, int start, bool *n_option)
{
	int	i;

	i = start;
	while (argv[i] && is_all_n_flags(argv[i]))
	{
		*n_option = true;
		i++;
	}
	return (i);
}

void	print_arguments(char **argv, int start)
{
	int	i;

	i = start;
	while (argv[i])
	{
		ft_putstr_fd(argv[i], 1);
		if (argv[i + 1])
			ft_putstr_fd(" ", 1);
		i++;
	}
}

int	ft_echo(char **argv)
{
	int		i;
	bool	n_option;

	i = 1;
	n_option = false;
	i = skip_n_flags(argv, i, &n_option);
	print_arguments(argv, i);
	if (!n_option)
		ft_putstr_fd("\n", 1);
	return (0);
}
