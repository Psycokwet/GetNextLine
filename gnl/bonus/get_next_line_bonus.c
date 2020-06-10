/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scarboni <scarboni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/05 10:38:18 by scarboni          #+#    #+#             */
/*   Updated: 2020/06/08 14:23:11 by scarboni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

#include <stdio.h>

size_t	ft_strlen(const char *s)
{
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

t_list *	get_current_node(t_list ** head, int fd)
{
	t_list *lst;

	if(!*head)
		*head = ft_lstnew(fd);
	lst = *head;
	while(1){
		if(!lst){
			lst = *head;
			*head = ft_lstnew(fd);
			if(head == NULL){
				*head = lst;
				return NULL;
			}
			(*head)->next = lst;
			lst = *head;
		}

		if(lst->fd_wip->fd == fd){
			return lst;
			}
		else{

			lst = lst->next;
		}
	}

}


int cut_line_n(char **line, pt_fd_read_wip fd_wip, size_t n_indice){
	if(!line)
		return -1;
	if(*line)
		free(*line);
	*line = malloc(sizeof(char) * (n_indice + 1));
	if(!*line)
		return -1;
	ft_strlcpy(*line, fd_wip->line_wip, n_indice + 1);
	char * tmp = ft_strdup(fd_wip->line_wip + n_indice + 1);
	free(fd_wip->line_wip);
	fd_wip->line_wip = tmp;
	fd_wip->size = fd_wip->size - (n_indice + 1);
	return 1;
}

int append_buffer(pt_fd_read_wip fd_wip, char* buffer, int ret_read){
	if(!fd_wip->line_wip){
		fd_wip->size = ret_read;
		fd_wip->line_wip = ft_strdup(buffer);
		if(!fd_wip->line_wip)
			return -1;
	}
	else
	{
		char *tmp = malloc(sizeof(char) * (fd_wip->size + BUFFER_SIZE + 1));
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

int read_full_line(t_list * current_node, size_t * n_indice, char **line){
	int ret_read = 1;
    char * buffer = (char*)malloc(BUFFER_SIZE * sizeof(char) + 1);
	if(!buffer)
		return -1;
    while(ret_read)
    {
		ret_read = read(current_node->fd_wip->fd, buffer, BUFFER_SIZE);
		if(ret_read == -1){
			free(buffer);
			return -1;
		}
		buffer[ret_read] = '\0';
		append_buffer(current_node->fd_wip, buffer, ret_read);
		if(ft_strchr(current_node->fd_wip->line_wip, '\n', n_indice)){
			free(buffer);
			return cut_line_n(line, current_node->fd_wip, *n_indice);
		}
		if(ret_read == 0){
			free(buffer);
			return cut_line_n(line, current_node->fd_wip, current_node->fd_wip->size) == 1? 0: -1;
		}
    }
	return -1;
}

int	get_next_line(int fd, char **line)
{
	static t_list * head;
	size_t n_indice;
	t_list * current_node;
	int return_value;
	current_node = get_current_node(&head, fd);
	if(current_node->fd_wip->line_wip){
		if(ft_strchr(current_node->fd_wip->line_wip, '\n', &n_indice)){
			int test = cut_line_n(line, current_node->fd_wip, n_indice);
			(void)test;
            return 1;
        }
	}
	return_value = read_full_line(current_node, &n_indice, line);
	if(return_value != 1){
		ft_lstdelnode(&head, fd);
	}
	return return_value;
}
