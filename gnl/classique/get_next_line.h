/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scarboni <scarboni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/05 11:14:23 by scarboni          #+#    #+#             */
/*   Updated: 2020/06/23 14:50:02 by scarboni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stddef.h>
# include <unistd.h>
# include <stdlib.h>

typedef struct	s_fd_read_wip
{
	int			fd;
	ssize_t		last_ret_read;
	ssize_t		size;
	char		*line_wip;
}				t_fd_read_wip;

int				ft_strchr(const char *s, int c, ssize_t *indice);
char			*ft_strdup(const char *src);
size_t			ft_strlcpy(char *dst, const char *src, size_t dstsize);
size_t			ft_strlen(const char *s);
int				cut_line_n(char **line, t_fd_read_wip *fd_wip);
int				append_buffer(t_fd_read_wip *fd_wip, char *buffer,
ssize_t ret_read);
int				read_full_line(t_fd_read_wip *fd_wip, char **line, char *buffer);
t_fd_read_wip	*get_current_wip(t_fd_read_wip *current_wip, int fd);
void			gnl_cleaning(int return_value, t_fd_read_wip **current_wip, char *buffer);
int				get_next_line(int fd, char **line);

#endif
