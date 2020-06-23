/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scarboni <scarboni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/05 10:38:18 by scarboni          #+#    #+#             */
/*   Updated: 2020/06/23 15:21:47 by scarboni         ###   ########.fr       */
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
			return (-1);
	}
	else
	{
		//printf("ALLOC TMP WITH SIZE [%d]\n", ((int)fd_wip->size + BUFFER_SIZE + 1));
		tmp = (char*)malloc(sizeof(char) * (unsigned long)(fd_wip->size + BUFFER_SIZE + 1));
		if (!tmp)
			return (-1);
		//printf("ALLOC WRITING IN TMP WITH SIZE [%ld] FROM WIP [%s]\n", fd_wip->size + 1, fd_wip->line_wip);
		ft_strlcpy(tmp, fd_wip->line_wip, (size_t)(fd_wip->size + 1));
		ft_strlcpy(tmp + fd_wip->size, buffer, BUFFER_SIZE + 1);
		//printf("ALLOC WRITING IN TMP WITH SIZE [%ld] FROM WIP [%s] OF SIZE [%d]\n", fd_wip->size + 1, buffer, BUFFER_SIZE + 1 );
		free(fd_wip->line_wip);
		fd_wip->line_wip = tmp;
		fd_wip->size += ret_read;
	}
	return (0);
}

int				read_full_line(t_fd_read_wip *fd_wip, char **line, char *buffer)
{
	int		cut_line_n_ret;
	
	cut_line_n_ret = 1;
	while (fd_wip->last_ret_read)
	{
	//printf("WRITE WITH READ IN BUFFER WITH SIZE [%d]\n", (BUFFER_SIZE));
		fd_wip->last_ret_read = read(fd_wip->fd, buffer, BUFFER_SIZE);
		if (fd_wip->last_ret_read < 0)
			return (-1);
	//printf("WRITE IN BUFFER WITH INDICE [%d]\n", fd_wip->last_ret_read);
		buffer[fd_wip->last_ret_read] = '\0';
		if(append_buffer(fd_wip, buffer, fd_wip->last_ret_read) == -1)
			return -1;
		cut_line_n_ret = cut_line_n(line, fd_wip);
		if (cut_line_n_ret != 2)
			return (cut_line_n_ret);
	}
	*line = fd_wip->line_wip;
	fd_wip->line_wip = NULL;
	return (0);
}

int			set_current_wip(t_fd_read_wip *current_wip, int fd)
{
	if (!current_wip)
		return (-1);
	if (current_wip->fd == fd)
		return (0);
	if (current_wip->line_wip)
		free(current_wip->line_wip);
	current_wip->fd = fd;
	current_wip->line_wip = NULL;
	current_wip->size = 0;
	current_wip->last_ret_read = -2;
	return (0);
}

void			gnl_cleaning(int return_value, t_fd_read_wip *current_wip, char *buffer)
{
	if (return_value != 1 && current_wip)
		if (current_wip->line_wip)
		{
			free(current_wip->line_wip);
			current_wip->line_wip = NULL;
		}
	if(buffer)
		free(buffer);
	buffer = NULL;
}

int				get_next_line(int fd, char **line)
{
	static t_fd_read_wip	current_wip;
	int						cut_line_n_ret;
	int						return_value;
	char					*buffer;

	if (!line)
		return (-1);
	if (*line)
		free(*line);
	if (BUFFER_SIZE <= 0 || fd < 0)
		return (-1);
	set_current_wip(&current_wip, fd);
	if (current_wip.line_wip)
	{
		cut_line_n_ret = cut_line_n(line, &current_wip);
		if (cut_line_n_ret != 2)
			return (cut_line_n_ret);
	}
	//printf("ALLOC BUFFER WITH SIZE [%d]\n", (BUFFER_SIZE + 1));
	if((buffer = (char*)malloc(sizeof(char) * (BUFFER_SIZE + 1))))
		return_value = read_full_line(&current_wip, line, buffer);
	else
		return_value = -1;
	
	gnl_cleaning(return_value, &current_wip, buffer);
	return (return_value);
}
