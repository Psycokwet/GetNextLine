/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scarboni <scarboni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/05 10:38:18 by scarboni          #+#    #+#             */
/*   Updated: 2020/06/24 07:33:00 by scarboni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdio.h>

int				append_buffer(t_fd_read_wip *fd_wip, char *buffer, ssize_t ret_read)
{
	char *tmp;

	tmp = NULL;
	if (!fd_wip->line_wip)
	{
		fd_wip->size = ret_read;
		fd_wip->line_wip = ft_strdup(buffer);
		if (!fd_wip->line_wip)
			return (-EXIT_FAILURE);
	}
	else
	{
		tmp = (char*)malloc(sizeof(char) * (unsigned long)(fd_wip->size + BUFFER_SIZE + 1));
		if (!tmp)
			return (-EXIT_FAILURE);
		ft_strlcpy(tmp, fd_wip->line_wip, (size_t)(fd_wip->size + 1));
		ft_strlcpy(tmp + fd_wip->size, buffer, BUFFER_SIZE + 1);
		free(fd_wip->line_wip);
		fd_wip->line_wip = tmp;
		fd_wip->size += ret_read;
	}
	return (APPEND_SUCCES);
}

int				read_full_line(t_fd_read_wip *fd_wip, char **line, char *buffer)
{
	int		cut_line_n_ret;

	cut_line_n_ret = 1;
	while (fd_wip->last_ret_read)
	{
		fd_wip->last_ret_read = read(fd_wip->fd, buffer, BUFFER_SIZE);
		if (fd_wip->last_ret_read < 0)
			return (-EXIT_FAILURE);
		buffer[fd_wip->last_ret_read] = '\0';
		if (append_buffer(fd_wip, buffer, fd_wip->last_ret_read) == -1)
			return (-EXIT_FAILURE);
		cut_line_n_ret = cut_line_n(line, fd_wip);
		if (cut_line_n_ret != LINE_NOT_COMPLETE)
			return (cut_line_n_ret);
	}
	*line = fd_wip->line_wip;
	fd_wip->line_wip = NULL;
	return (EXIT_READ_CLOSED);
}

void			set_current_wip(t_fd_read_wip *current_wip, int fd)
{
	if ((current_wip->fd == fd) && (current_wip->line_wip))
		return ;
	if (current_wip->line_wip)
		free(current_wip->line_wip);
	*current_wip = (struct s_fd_read_wip){fd, INIT_RET_READ, 0, NULL};
}

static void		gnl_cleaning(int const return_value,
				t_fd_read_wip *current_wip, char *buffer)
{
	if ((return_value != EXIT_READ_OPEN) && (current_wip->line_wip))
	{
		free(current_wip->line_wip);
		current_wip->line_wip = NULL;
	}
	if (buffer)
		free(buffer);
}

int				get_next_line(int fd, char **line)
{
	static t_fd_read_wip	current_wip;
	int						cut_line_n_ret;
	int						return_value;
	char					*buffer;

	return_value = -EXIT_FAILURE;
	if (!line)
		return (-EXIT_FAILURE);
	if (BUFFER_SIZE <= 0 || fd < 0)
		return (-EXIT_FAILURE);
	set_current_wip(&current_wip, fd);
	if (current_wip.line_wip)
	{
		cut_line_n_ret = cut_line_n(line, &current_wip);
		if (cut_line_n_ret != LINE_NOT_COMPLETE)
			return (cut_line_n_ret);
	}
	if ((buffer = (char*)malloc(sizeof(char) * (BUFFER_SIZE + 1))))
		return_value = read_full_line(&current_wip, line, buffer);
	gnl_cleaning(return_value, &current_wip, buffer);
	return (return_value);
}
