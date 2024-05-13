/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperez-a <bperez-a@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 17:12:50 by eagranat          #+#    #+#             */
/*   Updated: 2024/05/13 07:53:32 by bperez-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int ft_unset(t_program **program, char **argv)
{
    int i;
    int j;
    char *env_var;
    char **new_envp;

    i = 1;
    while (argv[i])
    {
        env_var = ft_strjoin(argv[i], "=");
        j = find_env_var((*program)->envp, env_var);
        if (j != -1)
        {
            int len = ft_array_len((*program)->envp) - 1;
            new_envp = (char **)malloc(sizeof(char *) * (len + 1)); // +1 for NULL
            int k = 0;
            int m = 0;
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
            free((*program)->envp[j]);
            free((*program)->envp);
            (*program)->envp = new_envp;
        }
        free(env_var);
        i++;
    }
    return 0;
}

