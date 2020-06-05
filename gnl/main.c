/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scarboni <scarboni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/05 10:38:18 by scarboni          #+#    #+#             */
/*   Updated: 2020/06/05 09:40:36 by scarboni         ###   ########.fr       */
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
	(void)argc;
	(void)argv;
	char ** line = (char**)malloc(sizeof(char*) * argc);
	int * fd = (int*)malloc(sizeof(int) * argc);
	int * openfd = (int*)malloc(sizeof(int) * argc);
	int stop = 1;

	for(int i = 1; i < argc; i++){
		fd[i] = open(argv[i], O_RDONLY);
		printf("main FD INIT : [%d] LINE : [%s]\n", fd[i], line[i]);
		openfd[i] = 1;
	}
	while(stop != argc)
	{
		stop = 1;
		for(int i = 1; i < argc; i++){
			if(openfd[i] == 1){
				openfd[i] = get_next_line(fd[i], &line[i]);
				printf("main FD RESULT: [%d] LINE : [%s]\n", fd[i], line[i]);
			}else
			{
				stop ++;
			}
		}
	}

	for(int i = 1; i < argc; i++){
		if(openfd[i] == -1){
			printf("main ERROR DETECTED FD : [%d], ARGV : [%s]", fd[i], argv[i]);
		}
		else{
			free(line[i]);
			close(fd[i]);
		}
	}

	free(openfd);
	free(line);
	free(fd);


	return (0);
}
