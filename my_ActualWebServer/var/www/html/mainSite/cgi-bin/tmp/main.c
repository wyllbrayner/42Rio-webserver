#include "get_next_line.h"
#include <stdio.h>

#include "get_next_line.h"
#include <stdio.h>

int    main()
{
//    printf("inicio | main\n");
    int     fd;
    fd = open("./teste2.txt", O_RDONLY);
//    fd = open("./vazio.txt", O_RDONLY);
//    fd = open("./text_test.txt", O_RDONLY);
    char    *ret;
    int     i = 1;

    if (fd == -1)
    {
        printf("open retornou erro com fd: %i \n", fd);
        return (1);
    }
//    ret = get_next_line(fd);
//    printf("retorno da get_next_line: %s\n", ret);
//	free(ret);
    while ((ret = get_next_line(fd)))
    {
//        printf("retorno da get_next_line: %s quando i: %i\n", ret, i);
        printf("%s\n", ret);
		free(ret);
        ret = NULL;
        i++;
//        if (i > 3)
//            break ;
    }
    if (fd >= 2)
	    close(fd);
//    printf("fim    | main\n");
    return (0);
}
