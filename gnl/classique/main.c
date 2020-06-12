/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scarboni <scarboni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/05 10:38:18 by scarboni          #+#    #+#             */
/*   Updated: 2020/06/12 20:05:01 by scarboni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "get_next_line.h"
#include <fcntl.h>

int		main(int argc, char **argv)
{
	char	**line;
	int		*fd;
	int		*openfd;
	int		i;
	int		stop;

	i = 0;
	line = (char**)malloc(sizeof(char*) * argc);
	if (!line)
		return (-1);
	fd = (int*)malloc(sizeof(int) * argc);
	if (!fd)
	{
		free(line);
		return (-1);
	}
	openfd = (int*)malloc(sizeof(int) * argc);
	if (!openfd)
	{
		free(fd);
		free(line);
		return (-1);
	}
	stop = 1;
	for (i = 1; i < argc; i++)
	{
		fd[i] = open(argv[i], O_RDONLY);
		if (fd[i] < 0)
			openfd[i] = 0;
		line[i] = NULL;
		printf("main FD INIT : [%d] \n", fd[i]);
		openfd[i] = (1);
	}
	while (stop != argc)
	{
		stop = 1;
		for (i = 1; i < argc; i++)
		{
			if (openfd[i] == 1)
			{
				openfd[i] = get_next_line(fd[i], &line[i]);
				if (openfd[i] != -1)
					printf("main end FD RESULT: [%d] LINE : [%s] OPEN? %d\n", fd[i], line[i], openfd[i]);
				else
					printf("Error encountered: [%d] OPEN? %d\n", fd[i], openfd[i]);
			}
			else
				stop ++;
		}
	}
	for (i = 1; i < argc; i++)
	{
		if (openfd[i] == -1){
			printf("main ERROR DETECTED FD : [%d], ARGV : [%s]", fd[i], argv[i]);
		}
		else
		{
			free(line[i]);
			close(fd[i]);
		}
	}
	free(openfd);
	free(line);
	free(fd);
	return (0);
}
