/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scarboni <scarboni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/05 10:38:18 by scarboni          #+#    #+#             */
/*   Updated: 2020/06/25 17:20:52 by scarboni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

t_list		*ft_lstnew(int fd)
{
	t_list *new_lst;

	new_lst = (t_list*)malloc(sizeof(t_list));
	if (!new_lst)
		return (NULL);
	*new_lst = (t_list){(t_fd_read_wip){fd, INIT_RET_READ, 0, NULL}, NULL};
	return (new_lst);
}

int			ft_strchr(const char *s, int c, ssize_t *indice)
{
	ssize_t	i;

	i = -1;
	while (s[++i])
		if ((unsigned char)s[i] == c)
		{
			*indice = i;
			return (ENDL_FOUND);
		}
	if ((unsigned char)s[i] == c)
	{
		*indice = i;
		return (ENDL_FOUND);
	}
	return (ENDL_NOT_FOUND);
}

char		*ft_strdup(const char *src)
{
	char	*dst;
	size_t	len;
	size_t	i;

	i = 0;
	while (src[i])
		i++;
	len = i + 1;
	dst = (char *)malloc(len * sizeof(char));
	if (dst != NULL)
		ft_strlcpy(dst, src, len);
	return (dst);
}

size_t		ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t		i;

	if (!src)
		return (0);
	if (!dst || dstsize <= 0)
	{
		i = 0;
		while (src[i])
			i++;
		return (i);
	}
	i = (size_t)-1;
	while (src[++i] && i < dstsize - 1)
		dst[i] = src[i];
	dst[i] = '\0';
	while (src[i])
		i++;
	return (i);
}

int			cut_line_n(char **line, t_fd_read_wip *fd_wip)
{
	ssize_t	n_indice;
	char	*tmp;

	n_indice = 0;
	if (!ft_strchr(fd_wip->line_wip, '\n', &n_indice))
		return (LINE_NOT_COMPLETE);
	*line = malloc(sizeof(char) * (unsigned long)(n_indice + 1));
	if (!(*line))
		return (-EXIT_FAILURE);
	ft_strlcpy(*line, fd_wip->line_wip, (size_t)n_indice + 1);
	tmp = ft_strdup(fd_wip->line_wip + n_indice + 1);
	free(fd_wip->line_wip);
	fd_wip->line_wip = tmp;
	fd_wip->size = fd_wip->size - (n_indice + 1);
	return (fd_wip->last_ret_read == 0 ? EXIT_READ_CLOSED : EXIT_READ_OPEN);
}
