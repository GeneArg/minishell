/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperez-a <bperez-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 17:12:50 by eagranat          #+#    #+#             */
/*   Updated: 2024/05/07 16:51:28 by bperez-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int ft_unset(t_program **program, char **argv)
{
	int i;
	int j;
	int k;
	char *env_var;
	char **new_envp;

	i = 1;
	while (argv[i])
	{
		env_var = ft_strjoin(argv[i], "=");
		j = find_env_var((*program)->envp, env_var);
		if (j != -1)
		{
			k = 0;
			new_envp = (char **)malloc(sizeof(char *) * (ft_array_len((*program)->envp)));
			while ((*program)->envp[k])
			{
				if (k != j)
					new_envp[k] = ft_strdup((*program)->envp[k]);
				k++;
			}
			new_envp[k] = NULL;
			free((*program)->envp[j]);
			free((*program)->envp);
			(*program)->envp = new_envp;
		}
		// else
		// {
		// 	printf("minishell: unset: `%s': not a valid identifier\n", argv[i]);
		// 	return (1);
		// }
		free(env_var);
		i++;
	}
	return (0);
}
