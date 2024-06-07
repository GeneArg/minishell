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

bool is_all_n_flags(char *arg)
{
    int j = 2;
    if (arg[0] != '-' || arg[1] != 'n')
        return false;
    while (arg[j] != '\0')
    {
        if (arg[j] != 'n')
            return false;
        j++;
    }
    return true;
}

int skip_n_flags(char **argv, int start, bool *n_option)
{
    int i = start;
    while (argv[i] && is_all_n_flags(argv[i]))
    {
        *n_option = true;
        i++;
    }
    return i;
}

void print_arguments(char **argv, int start)
{
    int i = start;
    while (argv[i])
    {
        ft_putstr_fd(argv[i], 1);
        if (argv[i + 1])
            ft_putstr_fd(" ", 1);
        i++;
    }
}

int ft_echo(char **argv)
{
    int i = 1;
    bool n_option = false;

    i = skip_n_flags(argv, i, &n_option);
    print_arguments(argv, i);
    if (!n_option)
        ft_putstr_fd("\n", 1);

    return 0;
}
