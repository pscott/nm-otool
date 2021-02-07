#include "nm.h"
#include <stdint.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

static inline Elf64_Shdr *elf_sheader(Elf64_Ehdr *hdr)
{
    return (Elf64_Shdr *)((char *)hdr + hdr->e_shoff); // scott maybe
}

static inline Elf64_Shdr *elf_section(Elf64_Ehdr *hdr, int idx)
{
    return &elf_sheader(hdr)[idx]; // scott int maybe
}

int handle_64(char *map_start, size_t size)
{
    Elf64_Ehdr *header;
    Elf64_Shdr *section_headers;
    Elf64_Shdr *symtab;
    size_t nums;
    Elf64_Shdr *strtab;

    if (size < (off_t)sizeof(Elf64_Ehdr))
    {
        ft_dprintf(2, "error: header file too small\n");
        return (EXIT_FAILURE);
    }
    header = (Elf64_Ehdr *)map_start;
    section_headers = (Elf64_Shdr *)((char *)map_start + header->e_shoff);
    strtab = NULL;
    for (int i = 0; i < header->e_shnum; i++)
    {
        if (i == header->e_shstrndx)
        {
            if (section_headers[i].sh_type == SHT_STRTAB)
            {

                strtab = (Elf64_Shdr *)(&section_headers[i]);
                // ft_printf("%u\n", strtab->sh_offset);
                // ft_printf("i: %d, strtab: %p\n", i, (void *)strtab);
            }
            else
            {
                ft_dprintf(2, "ERROR FORMAT");
            }
        }
        if (section_headers[i].sh_type == SHT_SYMTAB)
        {
            // ft_printf("i: %d, symtab: %p\n", i, &section_headers[i]);
            symtab = (Elf64_Shdr *)(char *)(&section_headers[i]);
            size = section_headers[i].sh_size;
            nums = size / section_headers[i].sh_entsize;
            // ft_printf("size: %d, num: %d, total: %d\n", size,
            // section_headers[i].sh_entsize, nums);
        }
    }

    if (strtab)
    {
        if (symtab)
        {
            Elf64_Sym *symbols = (Elf64_Sym *)(map_start + symtab->sh_offset);
            struct result_list *head;
            char type;

            for (size_t ii = 0; ii < nums; ii++)
            {
                Elf64_Sym sym = symbols[ii];
                if (sym.st_name)
                {
                    Elf64_Shdr *strtab = elf_section(header, symtab->sh_link);
                    char *name =
                        (char *)header + strtab->sh_offset + sym.st_name;
                    if (ELF64_ST_BIND(sym.st_info) != STB_LOCAL) // not correct condition !
                    {
                        type = 'T';
                        insert_elem(&head, new_elem(name, sym.st_value, type));
                    }
                }
            }
            print_list(head);
            free_list(head);
        }
        else
        {
            ft_dprintf(2, "error: no symtab found\n");
            return (EXIT_FAILURE);
        }
    }
    else
    {
        ft_dprintf(2, "error: no strtab found\n");
        return (EXIT_FAILURE);
    }
    return (EXIT_SUCCESS);
}

int elf64_check_supported(char *ptr, size_t file_size)
{
    Elf64_Ehdr *hdr;

    if (file_size < sizeof(Elf64_Ehdr))
    {
        return (EXIT_FAILURE);
    }
    hdr = (Elf64_Ehdr *)ptr;
    if (hdr->e_ident[EI_MAG0] != ELFMAG0)
        return (EXIT_FAILURE);
    if (hdr->e_ident[EI_MAG1] != ELFMAG1)
        return (EXIT_FAILURE);
    if (hdr->e_ident[EI_MAG2] != ELFMAG2)
        return (EXIT_FAILURE);
    if (hdr->e_ident[EI_MAG3] != ELFMAG3)
        return (EXIT_FAILURE);
    if (hdr->e_ident[EI_CLASS] != ELFCLASS64)
        return (EXIT_FAILURE);
    if (hdr->e_ident[EI_DATA] != ELFDATA2LSB)
        return (EXIT_FAILURE);
    // if (hdr->e_machine != EM_386) {
    // return (EXIT_FAILURE);
    // }
    if (hdr->e_ident[EI_VERSION] != EV_CURRENT)
        return (EXIT_FAILURE);
    // if (hdr->e_type != ET_REL && hdr->e_type != ET_EXEC) {
    // return (EXIT_FAILURE);
    // }
    return (EXIT_SUCCESS);
}

int nm(char *ptr, struct stat *buf)
{
    // if (elf32_check_supported(ptr, buf->st_size) == EXIT_SUCCESS) {
    // handle_32(ptr, buf->st_size);
    // } else if (elf64_check_supported(ptr, buf->st_size)) {
    if (elf64_check_supported(ptr, buf->st_size) == EXIT_SUCCESS)
    {
        handle_64(ptr, buf->st_size);
    }
    else
    {
        ft_dprintf(2, "Unsupported ELF format\n");
        return (EXIT_FAILURE);
    }
    return (EXIT_SUCCESS);
}

int try_close(int fd, char *file_name, int exit_value)
{
    if (close(fd) < 0)
    {
        ft_dprintf(2, "nm: error while closing '%s'\n", file_name);
        return (EXIT_FAILURE);
    }
    return (exit_value);
}

int main(int ac, char **av)
{
    int fd;
    struct stat buf;
    void *ptr;
    int ret;

    if (ac < 2)
    {
        fd = open(DEFAULT_FILE, O_RDONLY);
        if (fd < 1)
        {
            ft_dprintf(2, "nm: error while trying to open '%s'\n", DEFAULT_FILE);
        }
    }
    for (int i = 1; i < ac; i++)
    {
        fd = open(av[i], O_RDONLY);
        if (fd < 1)
        {
            ft_dprintf(2, "nm: error while trying to open '%s'\n", av[i]);
            return (EXIT_FAILURE);
        }
        if (fstat(fd, &buf) < 0)
        {
            ft_dprintf(2, "nm: error while trying to get stats from file '%s'\n",
                       av[i]);
            return (try_close(fd, av[i], EXIT_FAILURE));
        }
        if ((ptr = mmap(NULL, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) ==
            MAP_FAILED)
        {
            ft_dprintf(2, "nm: error while trying to map file '%s'\n", av[i]);
            return (try_close(fd, av[i], EXIT_FAILURE));
        }
        ret = nm(ptr, &buf);
        if (try_close(fd, av[i], ret) == EXIT_FAILURE)
        {
            return (EXIT_FAILURE);
        }
    }
    return (EXIT_SUCCESS);
}