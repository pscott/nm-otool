#include "nm.h"

struct result_list *new_elem(char *name, Elf64_Addr value, char type)
{
    struct result_list *res;

    if (!(res = malloc(sizeof(*res))))
        return (NULL);
    res->name = name;
    res->value = value;
    res->type = type;
    res->next = NULL;
    return (res);
}

void insert_elem(struct result_list **head, struct result_list *new)
{
    struct result_list *probe;
    struct result_list *last;

    if (!*head)
        *head = new;
    else if (ft_strcmp((*head)->name, new->name) >= 0)
    {
        new->next = *head;
        *head = new;
    } else {
        probe = (*head)->next;
        last = *head;
        while (probe)
        {
            if (ft_strcmp(probe->name, new->name) >= 0)
            {
                last->next = new;
                new->next = probe;
                return;
            }
            last = probe;
            probe = probe->next;
        }
        last->next = new;
    }
}

void print_list(struct result_list *head)
{
    while (head)
    {
        if (head->value == 0)
            ft_printf("%16c %c %s\n", ' ', head->type, head->name);
        else
            ft_printf("%016x %c %s\n", head->value, head->type, head->name);
        head = head->next;
    }
}

void free_list(struct result_list *head) {
    struct result_list *probe;
    struct result_list *next;

    if (head)
    {
        probe = head;
        while (probe) {
            next = probe->next;
            free(probe);
            probe = next;
        }
    }
}