/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array_len.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eagranat <eagranat@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 13:37:23 by eagranat          #+#    #+#             */
/*   Updated: 2024/05/04 13:43:40 by eagranat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	array_len(char **array)
{
	size_t	i;

	if (!array)
		return (0);
	if (!*array)
		return (0);
	i = 0;
	while (array[i])
		i++;
	return (i);
}