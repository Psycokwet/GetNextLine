/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scarboni <scarboni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/05 10:38:18 by scarboni          #+#    #+#             */
/*   Updated: 2020/06/16 10:57:41 by scarboni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int			ft_strchr(const char *s, int c, size_t *indice)
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
	printf("ALLOC WRITING IN WIP WITH SIZE [%ld]\n", len);
		
	dst = (char *)malloc(len * sizeof(char));
	if (dst != NULL)
		ft_strlcpy(dst, src, len);
	return (dst);
}

size_t		ft_strlcpy(char *dst, const char *src, size_t dstsize)
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

size_t		ft_strlen(const char *s)
{
	size_t i;

	if (!s)
		return (-1);
	i = 0;
	while (s[i])
		i++;
	return (i);
}

int			cut_line_n(char **line, t_fd_read_wip *fd_wip)
{
	size_t	n_indice;
	char	*tmp;

	n_indice = 0;
	if (!ft_strchr(fd_wip->line_wip, '\n', &n_indice))
		return (2);
	*line = malloc(sizeof(char) * (n_indice + 1));
	if (!(*line))
		return (-1);
	ft_strlcpy(*line, fd_wip->line_wip, n_indice + 1);
	tmp = ft_strdup(fd_wip->line_wip + n_indice + 1);
	free(fd_wip->line_wip);
	fd_wip->line_wip = tmp;
	fd_wip->size = fd_wip->size - (n_indice + 1);
	return (fd_wip->last_ret_read == 0 ? 0 : 1);
}
