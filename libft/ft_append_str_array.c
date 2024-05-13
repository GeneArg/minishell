/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_append_str_array.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperez-a <bperez-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 11:02:27 by bperez-a          #+#    #+#             */
/*   Updated: 2024/05/13 11:02:43 by bperez-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_append_str_array(char ***array, char *str)
{
	int		len;
	char	**new_array;

	len = 0;
	if (**array)
	{
		while ((*array)[len])
			len++;
	}
	new_array = (char **)malloc(sizeof(char *) * (len + 2));
	for (int i = 0; i < len; i++)
	{
		new_array[i] = (*array)[i];
		(*array)[i] = NULL;
	}
	new_array[len] = ft_strdup(str);
	new_array[len + 1] = NULL;
	while (len >= 0)
	{
		free((*array)[len]);
		len--;
	}
	free(*array);
	*array = new_array;
}
