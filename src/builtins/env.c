/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperez-a <bperez-a@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 20:39:38 by bperez-a          #+#    #+#             */
/*   Updated: 2024/05/12 20:39:44 by bperez-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int ft_env(t_program **program)
{
	for (int i = 0; (*program)->envp[i]; i++)
	{
		if (ft_strncmp((*program)->envp[i], "_=", 2)
			&& ft_strncmp((*program)->envp[i], "?=", 2))
			printf("%s\n", (*program)->envp[i]);
	}
	return (0);
}
