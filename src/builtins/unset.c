/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperez-a <bperez-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 17:12:50 by eagranat          #+#    #+#             */
/*   Updated: 2024/05/27 10:30:30 by bperez-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	remove_env_var(t_program **program, int j)
{
	int		len;
	int		k;
	int		m;
	char	**new_envp;

	len = ft_array_len((*program)->envp) - 1;
	new_envp = (char **)malloc(sizeof(char *) * (len + 1));
	if (!new_envp)
		return (FAILURE);
	k = 0;
	m = 0;
	while ((*program)->envp[k])
	{
		if (k != j)
		{
			new_envp[m] = ft_strdup((*program)->envp[k]);
			m++;
		}
		k++;
	}
	new_envp[m] = NULL;
	ft_free_array((*program)->envp);
	(*program)->envp = new_envp;
	return (SUCCESS);
}

int	ft_unset(t_program **program, char **argv)
{
	int		i;
	int		j;
	char	*env_var;

	i = 1;
	while (argv[i])
	{
		env_var = ft_strjoin(argv[i], "=");
		j = find_env_var((*program)->envp, env_var);
		if (j != -1)
		{
			if (remove_env_var(program, j) == FAILURE)
			{
				free(env_var);
				return (FAILURE);
			}
		}
		free(env_var);
		i++;
	}
	return (SUCCESS);
}
