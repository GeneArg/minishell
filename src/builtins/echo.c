/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperez-a <bperez-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 15:52:31 by eagranat          #+#    #+#             */
/*   Updated: 2024/05/13 09:55:12 by bperez-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// char *remove_quotes(char *str)
// {
//     int len = strlen(str);
//     int i;

//     // Check for a quote at the beginning and remove it
//     if (str[0] == '\'' || str[0] == '\"')
//     {
//         for (i = 0; i < len; i++)
//         {
// 			printf("here1\n");
//             str[i] = str[i+1];
//         }
//     }

//     // Update the length after potentially removing a quote at the beginning
//     len = strlen(str);

//     // Check for a quote at the end and remove it
//     if (str[len-1] == '\'' || str[len-1] == '\"')
//     {
// 		printf("here2\n");
//         str[len-1] = '\0';
//     }

//     return str;
// }

int	ft_echo(char **argv)
{
	int	i;
	// char	*arg;
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
		// arg = remove_quotes(argv[i]);
		ft_putstr_fd(argv[i], 1);
		if (argv[i + 1])
			ft_putstr_fd(" ", 1);
		else if (!argv[i + 1] && !n_option)
			ft_putstr_fd("\n", 1);
		i++;
	}
	return (0);
}
