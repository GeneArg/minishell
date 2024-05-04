/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_valid_int.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eagranat <eagranat@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 13:39:52 by eagranat          #+#    #+#             */
/*   Updated: 2024/05/04 13:41:40 by eagranat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_is_valid_integer(char *str)
{
	char	*tmp;

	tmp = str;
	if (*tmp == '-' || *tmp == '+')
		tmp++;
	if (!*tmp)
		return (0);
	while (*tmp)
	{
		if (!(ft_isdigit(*tmp)))
			return (0);
		tmp++;
	}
	return (1);
}