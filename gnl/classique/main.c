#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include "get_next_line.h"


int         main(int ac, char **av)
{
    int     fd;
    char    *line;

    if (ac < 2)
        return (EXIT_FAILURE);
    if ((fd = open(av[1], O_RDONLY)) < 0)
    {
        dprintf(STDERR_FILENO, "%s", "failed to open the file for reading");
        return (EXIT_FAILURE);
    }
    while (get_next_line(fd, &line) > 0)
    {
        printf("-> %s\n", line);
        free(line);
    }
    free(line);
    close(fd);
    return (EXIT_SUCCESS);
}
