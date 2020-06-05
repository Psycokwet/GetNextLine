/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scarboni <scarboni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/05 11:14:23 by scarboni          #+#    #+#             */
/*   Updated: 2020/06/05 09:51:56 by scarboni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stddef.h>
# include <unistd.h>
# include <stdlib.h>

/*
** read, malloc, free authorized
*/


typedef struct	s_fd_read_wip
{
	int		    	fd;
	size_t	    	size;
	char	        *line_wip;
}				t_fd_read_wip, *pt_fd_read_wip;

typedef struct	s_list
{
	pt_fd_read_wip	fd_wip;
	struct s_list	*next;
}				t_list;



int	get_next_line(int fd, char **line);
t_list *	get_current_node(t_list ** head, int fd);


// from libft

int	ft_strchr(const char *s, int c, size_t * indice);
char	*ft_strdup(const char *src);

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize);
size_t	ft_strlen(const char *s);
//lst

void	ft_lstdelone(t_list *lst);
t_list	*ft_lstnew(int fd);


#endif