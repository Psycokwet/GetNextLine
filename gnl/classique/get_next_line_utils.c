/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scarboni <scarboni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/05 10:38:18 by scarboni          #+#    #+#             */
/*   Updated: 2020/06/12 19:55:19 by scarboni         ###   ########.fr       */
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

size_t	ft_strlen(const char *s)
{
	if(!s)
		return -1;
	size_t i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

int				cut_line_n(char **line, t_fd_read_wip *fd_wip)
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

int				append_buffer(t_fd_read_wip *fd_wip, char *buffer, int ret_read)
{
	char *tmp;

	tmp = NULL;
	if (!fd_wip->line_wip)
	{
		fd_wip->size = ret_read;
		fd_wip->line_wip = ft_strdup(buffer);
		if (!fd_wip->line_wip)
			return (-1);
	}
	else
	{
		tmp = (char*)malloc(sizeof(char) * ((int)fd_wip->size + 33));
		if (!tmp)
			return (-1);
		ft_strlcpy(tmp, fd_wip->line_wip, fd_wip->size + 1);
		ft_strlcpy(tmp + fd_wip->size, buffer, BUFFER_SIZE + 1);
		free(fd_wip->line_wip);
		fd_wip->line_wip = tmp;
		fd_wip->size += ret_read;
	}
	return (0);
}

int				read_full_line(t_fd_read_wip *fd_wip, char **line)
{
	int		cut_line_n_ret;
	char	*buffer;

	cut_line_n_ret = 1;
	buffer = NULL;
	buffer = (char*)malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!buffer)
		return (-1);
	while (fd_wip->last_ret_read)
	{
		fd_wip->last_ret_read = read(fd_wip->fd, buffer, BUFFER_SIZE);
		if (fd_wip->last_ret_read == -1)
		{
			free(buffer);
			return (-1);
		}
		buffer[fd_wip->last_ret_read] = '\0';
		append_buffer(fd_wip, buffer, fd_wip->last_ret_read);
		cut_line_n_ret = cut_line_n(line, fd_wip);
		if (cut_line_n_ret != 2)
		{
			free(buffer);
			return (cut_line_n_ret);
		}
	}
	free(buffer);
	*line = fd_wip->line_wip;
	fd_wip->line_wip = NULL;
	return (0);
}

t_fd_read_wip	*get_current_wip(t_fd_read_wip *current_wip, int fd)
{
	if (current_wip && current_wip->fd == fd)
		return (current_wip);
	if (current_wip)
	{
		if (current_wip->line_wip)
			free(current_wip->line_wip);
		free(current_wip);
	}
	current_wip = (t_fd_read_wip*)malloc(sizeof(t_fd_read_wip));
	if (!current_wip)
		return (NULL);
	current_wip->fd = fd;
	current_wip->line_wip = NULL;
	current_wip->size = 0;
	current_wip->last_ret_read = -2;
	return (current_wip);
}

int				get_next_line(int fd, char **line)
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
