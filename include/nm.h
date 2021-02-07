#ifndef NM_H
#define NM_H

#define DEFAULT_FILE "a.out"
#include "libft.h"
#include <elf.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

struct result_list {
    char                *name;
    Elf64_Addr          value;
    struct result_list  *next;
    char                type;
};

void free_list(struct result_list *head);
void print_list(struct result_list *head);
struct result_list *new_elem(char *name, Elf64_Addr value, char type);
void insert_elem(struct result_list **head, struct result_list *new);

#endif
