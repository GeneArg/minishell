/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperez-a <bperez-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 20:39:38 by bperez-a          #+#    #+#             */
/*   Updated: 2024/05/27 10:17:37 by bperez-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_env(t_program **program)
{
	int	i;

	i = 0;
	while ((*program)->envp[i])
	{
		if (ft_strncmp((*program)->envp[i], "_=", 2)
			&& ft_strncmp((*program)->envp[i], "?=", 2))
			printf("%s\n", (*program)->envp[i]);
		i++;
	}
	return (SUCCESS);
}
