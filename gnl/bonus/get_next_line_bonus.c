/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scarboni <scarboni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/05 10:38:18 by scarboni          #+#    #+#             */
/*   Updated: 2020/06/25 14:37:02 by scarboni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

int				append_buffer(t_fd_read_wip *fd_wip, char *buffer,
				ssize_t ret_read)
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
		tmp = (char*)malloc(sizeof(char) *
		(unsigned long)(fd_wip->size + BUFFER_SIZE + 1));
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
		printf("BEFORE READ : %zd \n", fd_wip->last_ret_read);
		printf("BEFORE READ : %s \n", buffer);
		printf("BEFORE READ : %d \n", fd_wip->fd);
		fd_wip->last_ret_read = read(fd_wip->fd, buffer, BUFFER_SIZE);
		printf("AFTER READ : %zd \n", fd_wip->last_ret_read);
		if (fd_wip->last_ret_read < 0)
			return (-EXIT_FAILURE);
		printf("REAFTER READ : %zd \n", fd_wip->last_ret_read);
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

t_list	*ft_lstnew(int fd)
{
	t_list *new_lst;

	new_lst = (t_list*)malloc(sizeof(t_list));
	if (!new_lst){
		return (NULL);
	}
	new_lst = &(t_list){(t_fd_read_wip){fd, INIT_RET_READ, 0, NULL}, NULL};
	printf("new_lst->fd_wip->last_ret_read %zd.\n", new_lst->fd_wip.last_ret_read);
	return (new_lst);
}

void	set_summary(t_list_summary *summary, int fd)
{
	if(!summary->head)
		summary->head = ft_lstnew(fd);
	summary->current = summary->head;
	summary->prev = NULL;
	while (summary->current && !(summary->current->fd_wip.fd == fd))
	{
		summary->prev = summary->current;
		summary->current = summary->current->next;
	}
	if (!summary->current){
		summary->current = ft_lstnew(fd);
		summary->prev->next = summary->current;
	}
}

static void		gnl_cleaning(int const return_value,
				t_list_summary *summary, char *buffer)
{
	if (return_value != EXIT_READ_OPEN)
	{
		if (summary->current->fd_wip.line_wip)
			free(summary->current->fd_wip.line_wip);
		summary->prev->next = summary->current->next;
		free(summary->current);
		summary->current = NULL;
	}
	if (buffer)
		free(buffer);
}

int				get_next_line(int fd, char **line)
{
	static t_list_summary	summary;
	int						cut_line_n_ret;
	int						return_value;
	char					*buffer;

	return_value = -EXIT_FAILURE;
	if (!line)
		return (-EXIT_FAILURE);
	if (BUFFER_SIZE <= 0 || fd < 0)
		return (-EXIT_FAILURE);
	set_summary(&summary, fd);
	if (summary.current->fd_wip.line_wip)
	{
		cut_line_n_ret = cut_line_n(line, &summary.current->fd_wip);
		if (cut_line_n_ret != LINE_NOT_COMPLETE)
			return (cut_line_n_ret);
	}
	if ((buffer = (char*)malloc(sizeof(char) * (BUFFER_SIZE + 1)))){
		printf("WTF BUFFER ???%d\n", (BUFFER_SIZE + 1));
		buffer[0] = '\0';
		printf("WTF BUFFER ???%s\n", buffer);
		return_value = read_full_line(&summary.current->fd_wip, line, buffer);
	}
	gnl_cleaning(return_value, &summary, buffer);
	return (return_value);
}
