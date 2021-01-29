#include "nm.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdint.h>

int nm(char *ptr, struct stat *buf) {
    char class;

    if (buf->st_size < 5) {
        ft_dprintf(2, "Could not read magic number\n");
        return (EXIT_FAILURE);
    }
    if (!(ptr[EI_MAG0] == ELFMAG0 && 
            ptr[EI_MAG1] == ELFMAG1 && 
            ptr[EI_MAG2] == ELFMAG2 && 
            ptr[EI_MAG3] == ELFMAG3
        ))
    {
        ft_dprintf(2, "error: file is not ELF\n");
        return (EXIT_FAILURE);
    }
    class = ptr[EI_CLASS];
    if (class == ELFCLASS32) {
        ;
    } else if (class == ELFCLASS64) {
        ;
    } else {
        ft_dprintf(2, "File class '%d' not supported\n", class);
        return (EXIT_FAILURE);
    }
    return (EXIT_SUCCESS);
}

int try_close(int fd, char *file_name, int exit_value) {
    if (close(fd) < 0) {
        ft_dprintf(2, "nm: error while closing '%s'\n", file_name);
        return (EXIT_FAILURE);
    }
    return (exit_value);
}

int main(int ac, char **av) {
    int fd;
    struct stat buf;
    void *ptr;
    int ret;

    if (ac < 2)
    {
        fd = open(DEFAULT_FILE, O_RDONLY);
        if (fd < 1) {
            ft_dprintf(2, "nm: error while trying to open '%s'\n", DEFAULT_FILE);
        }
    }
    for (int i = 1; i < ac; i++) {
        fd = open(av[i], O_RDONLY);
        if (fd < 1) {
            ft_dprintf(2, "nm: error while trying to open '%s'\n", av[i]);
            return (EXIT_FAILURE);
        }
        if (fstat(fd, &buf) < 0) {
            ft_dprintf(2, "nm: error while trying to get stats from file '%s'\n", av[i]);
            return (try_close(fd, av[i], EXIT_FAILURE));
        }
        if ((ptr = mmap(NULL, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
        {
            ft_dprintf(2, "nm: error while trying to map file '%s'\n", av[i]);
            return (try_close(fd, av[i], EXIT_FAILURE));
        }
        ret = nm(ptr, &buf);
        if (try_close(fd, av[i], ret) == EXIT_FAILURE) {
            return (EXIT_FAILURE);
        }
    }
    return (EXIT_SUCCESS);
}