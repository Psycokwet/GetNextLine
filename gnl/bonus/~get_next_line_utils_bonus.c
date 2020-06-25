/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scarboni <scarboni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/05 10:38:18 by scarboni          #+#    #+#             */
/*   Updated: 2020/06/05 15:46:43 by scarboni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

#include<stdio.h>

// from libft
int	ft_strchr(const char *s, int c, size_t * indice)
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
    ssize_t len = ft_strlen(src) + 1;
	dst = (char *)malloc(len * sizeof(char));
	if (dst != NULL)
		ft_strlcpy(dst, src, len);
	return (dst);
}

void	ft_lstdelnode(t_list **head, int fd)
{
	t_list * to_delete;
	t_list * parent;

	if (!*head)
		return ;
	if((*head)->fd_wip->fd == fd)
	{
		to_delete = *head;
		head = &(*head)->next;
	}else{
		parent = *head;
		to_delete = (*head)->next;
		while(to_delete && to_delete->fd_wip->fd != fd){
			parent = to_delete;
			to_delete = to_delete->next;
		}
		if(!to_delete)
			return;
		parent->next = to_delete->next;
	}
	if (to_delete->fd_wip)
	{
		if (to_delete->fd_wip->line_wip)
		{
			free(to_delete->fd_wip->line_wip);
			to_delete->fd_wip->line_wip = NULL;
		}
		free(to_delete->fd_wip);
		to_delete->fd_wip = NULL;
	}
	free(to_delete);
}


t_list	*ft_lstnew(int fd)
{
	t_list *new_lst;
	pt_fd_read_wip fd_wip;

	fd_wip = malloc(sizeof(t_fd_read_wip));
	if (!fd_wip)
		return (NULL);
	fd_wip->fd = fd;
	fd_wip->line_wip = NULL;
	new_lst = (t_list*)malloc(sizeof(t_list));
	if (!new_lst){
		free(fd_wip);
		return (NULL);
	}
	new_lst->fd_wip = fd_wip;
	new_lst->next = NULL;
	return (new_lst);
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