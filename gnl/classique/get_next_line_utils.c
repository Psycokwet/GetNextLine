/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scarboni <scarboni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/05 10:38:18 by scarboni          #+#    #+#             */
/*   Updated: 2020/06/12 19:11:15 by scarboni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

#include<stdio.h>

// from libft
int	ft_strchr(const char *s, int c, size_t *indice)
{
	size_t	i;

	i = -1;
	while (s[++i])
		if ((unsigned char)s[i] == c)
		{
			*indice = i;
			return (1);
		}
	if ((unsigned char)s[i] == c)
	{
		*indice = i;
		return (1);
	}
	return (0);
}

char		*ft_strdup(const char *src)
{
    char	*dst;
    ssize_t len;
	len = ft_strlen(src) + 1;
	dst = (char *)malloc(len * sizeof(char));
	if (dst != NULL)
		ft_strlcpy(dst, src, len);
	return (dst);
}

size_t			ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	int		i;

	if (!src)
		return (0);
	if (!dst || dstsize == 0)
		return (ft_strlen(src));
	i = -1;
	while (src[++i] && i < ((int)dstsize) - 1)
		dst[i] = src[i];
	dst[i] = '\0';
	while (src[i])
		i++;
	return (i);
}
