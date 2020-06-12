/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scarboni <scarboni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/05 10:38:18 by scarboni          #+#    #+#             */
/*   Updated: 2020/06/12 19:37:23 by scarboni         ###   ########.fr       */
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

int	get_next_line(int fd, char **line)
{
	static t_fd_read_wip	*current_wip;
	int						cut_line_n_ret;
	int						return_value;

	if (!line)
		return (-1);
	if (*line)
		free(*line);
	current_wip = get_current_wip(current_wip, fd);
	if (!current_wip)
		return (-1);
	if (current_wip->line_wip)
	{
		cut_line_n_ret = cut_line_n(line, current_wip);
		if (cut_line_n_ret != 2)
			return (cut_line_n_ret);
	}
	return_value = read_full_line(current_wip, line);
	if (return_value != 1)
		if (current_wip)
		{
			if (current_wip->line_wip)
				free(current_wip->line_wip);
			free(current_wip);
		}
	return (return_value);
}
