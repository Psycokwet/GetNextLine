/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scarboni <scarboni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/05 10:38:18 by scarboni          #+#    #+#             */
/*   Updated: 2020/06/12 19:58:01 by scarboni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

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
