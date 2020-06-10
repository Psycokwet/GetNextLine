/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scarboni <scarboni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/05 10:38:18 by scarboni          #+#    #+#             */
/*   Updated: 2020/06/10 20:18:48 by scarboni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

#include <stdio.h>

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


/*
** read, malloc, free authorized
*/

/*
** Ecrivez une fonction qui retourne une ligne lue
** depuis un file descriptor, sans le retour à la
** ligne
**
** 1 : Une ligne a été lue
** 0 : La fin de fichier a été atteinte
** -1 : Une erreur est survenue
*/
//
int cut_line_n(char **line, pt_fd_read_wip fd_wip){
	size_t n_indice = 0;
	if(!ft_strchr(fd_wip->line_wip, '\n', &n_indice))
		return 2;
	
	*line = malloc(sizeof(char) * (n_indice + 1));
	if(!(*line))
		return -1;
	ft_strlcpy(*line, fd_wip->line_wip, n_indice + 1);
	char * tmp = ft_strdup(fd_wip->line_wip + n_indice + 1);
	free(fd_wip->line_wip);
	fd_wip->line_wip = tmp;
	fd_wip->size = fd_wip->size - (n_indice + 1);
	return fd_wip->last_ret_read == 0 ? 0 : 1;
}
//
int append_buffer(pt_fd_read_wip fd_wip, char* buffer, int ret_read){
	char *tmp;
	tmp = NULL;
	if(!fd_wip->line_wip){
		fd_wip->size = ret_read;
		fd_wip->line_wip = ft_strdup(buffer);
		if(!fd_wip->line_wip)
			return -1;
	}
	else{
		tmp = (char*)malloc(sizeof(char) * ((int)fd_wip->size + 33));
		if(!tmp)
			return -1;
		ft_strlcpy(tmp, fd_wip->line_wip, fd_wip->size + 1);
		ft_strlcpy(tmp + fd_wip->size, buffer, BUFFER_SIZE + 1);
		free(fd_wip->line_wip);
		fd_wip->line_wip = tmp;
		fd_wip->size += ret_read;
	}
	return 0;
}
//
int read_full_line(pt_fd_read_wip fd_wip, char **line){
	int cut_line_n_ret = 1;
    char * buffer;
	buffer = NULL;
	buffer = (char*)malloc((BUFFER_SIZE + 1) * sizeof(char));
	if(!buffer)
		return -1;
    while(fd_wip->last_ret_read){
		fd_wip->last_ret_read = read(fd_wip->fd, buffer, BUFFER_SIZE);
		if(fd_wip->last_ret_read == -1){
			free(buffer);
			return -1;
		}
		buffer[fd_wip->last_ret_read] = '\0';
		append_buffer(fd_wip, buffer, fd_wip->last_ret_read);
		cut_line_n_ret = cut_line_n(line, fd_wip);
		if(cut_line_n_ret != 2){
			free(buffer);
			return cut_line_n_ret;
		}
    }
	free(buffer);
	*line = fd_wip->line_wip;
	fd_wip->line_wip = NULL;
	return 0;
}

//
pt_fd_read_wip get_current_wip(pt_fd_read_wip current_wip, int fd){
	if(current_wip && current_wip->fd == fd)
		return current_wip;
	if(current_wip){
		if(current_wip->line_wip)
			free(current_wip->line_wip);
		free(current_wip);
	}
	current_wip = (pt_fd_read_wip)malloc(sizeof(t_fd_read_wip));
	if(!current_wip)
		return NULL;
	current_wip->fd = fd;
	current_wip->line_wip = NULL;
	current_wip->size = 0;
	current_wip->last_ret_read = -2;
	return current_wip;
}



int	get_next_line(int fd, char **line){
	static pt_fd_read_wip current_wip;
	int cut_line_n_ret;
	int return_value;
	if(!line)
		return -1;
	if(*line)
		free(*line);
	current_wip = get_current_wip(current_wip, fd);
	if(!current_wip)
		return -1;
	if(current_wip->line_wip){
		cut_line_n_ret = cut_line_n(line, current_wip);
		if(cut_line_n_ret != 2){
            return cut_line_n_ret;
        }
	}
	return_value = read_full_line(current_wip, line);
	if(return_value != 1){
		if(current_wip){
			if(current_wip->line_wip)
				free(current_wip->line_wip);
			free(current_wip);
		}
	}
	return return_value;
}