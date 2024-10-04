#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <elf.h>

static const char *get_elf_class(unsigned int elf_class)
{
  static char buff[32];
  switch (elf_class)
  {
    case ELFCLASSNONE: return "None";
    case ELFCLASS32:   return "ELF32";
    case ELFCLASS64:   return "ELF64";
    default:
		       snprintf(buff,sizeof(buff),"Unknown: %x",elf_class);
		       return buff;
  }
}

static const char *get_elf_data(unsigned int elf_data)
{
  switch (elf_data)
  {
    case ELFDATA2LSB: return "2's complement, little endian";
    case ELFDATA2MSB: return "2's complement, big endian";
    default: return "Unknown";
  }
}

static const char *get_elf_osabi(unsigned char osabi)
{
    switch (osabi) {
        case ELFOSABI_SYSV:     return "UNIX - System V";
        case ELFOSABI_HPUX:     return "HP-UX";
        case ELFOSABI_NETBSD:   return "NetBSD";
        case ELFOSABI_LINUX:    return "Linux";
        case ELFOSABI_SOLARIS:  return "Solaris";
        case ELFOSABI_AIX:      return "AIX";
        case ELFOSABI_IRIX:     return "IRIX";
        case ELFOSABI_FREEBSD:  return "FreeBSD";
        case ELFOSABI_OPENBSD:  return "OpenBSD";
        case ELFOSABI_ARM:      return "ARM";
        case ELFOSABI_STANDALONE: return "Standalone App";
        default:                return "Unknown";
    }
}

static const char *get_elf_type(unsigned int e_type)
{
  static char buff[32];
  switch (e_type)
  {
    case ET_NONE: return "No file type";
    case ET_REL:  return "Relocated file";
    case ET_EXEC: return "Executable file";
    case ET_DYN:  return "Shared object file";
    case ET_CORE: return "Core file";
    default:
		  snprintf(buff,sizeof(buff),"Unknown: %x",e_type);
		  return buff;
  }
}

static const char *get_elf_machine(unsigned int e_machine)
{
  static char buff[32];
  switch (e_machine)
  {
    case EM_NONE:   return "No Machine";
    case EM_M32:    return "AT&T WE 32100";
    case EM_SPARC:  return "SPARC";
    case EM_386:    return "Intel 80386";
    case EM_68K:    return "Motorola 68000";
    case EM_ARM:    return "ARM";
    case EM_X86_64: return "AArch64";
    default:
		    snprintf(buff,sizeof(buff),"Unknown: %x",e_machine);
		    return buff;
  }
}

static const char *get_section_type(unsigned int sh_type) 
{
    static char buff[32];

    switch (sh_type) {
        case SHT_NULL: return "NULL";
        case SHT_PROGBITS: return "PROGBITS";
        case SHT_SYMTAB: return "SYMTAB";
        case SHT_STRTAB: return "STRTAB";
        case SHT_RELA: return "RELA";
        case SHT_HASH: return "HASH";
        case SHT_DYNAMIC: return "DYNAMIC";
        case SHT_NOTE: return "NOTE";
        case SHT_NOBITS: return "NOBITS";
        case SHT_REL: return "REL";
        case SHT_SHLIB: return "SHLIB";
        case SHT_DYNSYM: return "DYNSYM";
        case SHT_LOOS: return "OS Specific";
        case SHT_HIOS: return "OS Specific";
        case SHT_LOPROC: return "Processor Specific";
        case SHT_HIPROC: return "Processor Specific";
        case SHT_GNU_HASH: return "GNU_HASH";
        case SHT_INIT_ARRAY: return "INIT_ARRAY";
        case SHT_FINI_ARRAY: return "FINI_ARRAY";
        case SHT_PREINIT_ARRAY: return "PREINIT_ARRAY";
        case SHT_GROUP: return "GROUP";
        case SHT_SYMTAB_SHNDX: return "SYMTAB_SHNDX";
        default:
            snprintf(buff, sizeof(buff), "Unknown: %x", sh_type);
            return buff;
    }
}

static const char *get_section_flags(unsigned int flags) 
{
    static char buff[32];
    snprintf(buff, sizeof(buff), "0x%x", flags);
    return buff;
}

static const char *get_section_name(int index, Elf64_Shdr *shdrs, char *strtab) 
{
    return &strtab[shdrs[index].sh_name];
}


static void print_ehdr(Elf64_Ehdr *ehdr) 
{
    printf("ELF Header:\n");
    printf("  Magic:   ");
    for (size_t cnt = 0; cnt < EI_NIDENT; ++cnt) 
    {
        printf(" %02x", ehdr->e_ident[cnt]);
    }
    printf("\n");
    printf("  Class:                             %s\n", get_elf_class(ehdr->e_ident[EI_CLASS]));
    printf("  Data:                              %s\n", get_elf_data(ehdr->e_ident[EI_DATA]));
    printf("  Version:                           %d (current)\n", ehdr->e_ident[EI_VERSION]);
    printf("  OS/ABI:                            %s\n", get_elf_osabi(ehdr->e_ident[EI_OSABI]));
    printf("  ABI Version:                       %d\n", ehdr->e_ident[EI_ABIVERSION]);
    printf("  Type:                              %s\n", get_elf_type(ehdr->e_type));
    printf("  Machine:                           %s\n", get_elf_machine(ehdr->e_machine));
    printf("  Version:                           0x%x\n", ehdr->e_version);
    printf("  Entry point address:               0x%lx\n",ehdr->e_entry);
    printf("  Start of program headers:          %ld (bytes into file)\n", ehdr->e_phoff);
    printf("  Start of section headers:          %ld (bytes into file)\n", ehdr->e_shoff);
    printf("  Flags:                             0x%x\n", ehdr->e_flags);
    printf("  Size of this header:               %u (bytes)\n", ehdr->e_ehsize);
    printf("  Size of program headers:           %u (bytes)\n", ehdr->e_phentsize);
    printf("  Number of program headers:         %u\n", ehdr->e_phnum);
    printf("  Size of section headers:           %u (bytes)\n", ehdr->e_shentsize);
    printf("  Number of section headers:         %u\n", ehdr->e_shnum);
    printf("  Section header string table index: %u\n", ehdr->e_shstrndx);
}

static void print_shdr(int fd, Elf64_Ehdr *ehdr) 
{
    Elf64_Shdr *shdrs = malloc(ehdr->e_shentsize * ehdr->e_shnum);
    if (shdrs == NULL) 
    {
        perror("malloc()");
        exit(EXIT_FAILURE);
    }

    lseek(fd, ehdr->e_shoff, SEEK_SET);
    read(fd, shdrs, ehdr->e_shentsize * ehdr->e_shnum);

    Elf64_Shdr *shstrhdr = &shdrs[ehdr->e_shstrndx];
    char *strtab = malloc(shstrhdr->sh_size);
    if (strtab == NULL)
    {
        perror("malloc()");
        free(shdrs);
        exit(EXIT_FAILURE);
    }

    lseek(fd, shstrhdr->sh_offset, SEEK_SET);
    read(fd, strtab, shstrhdr->sh_size);

    printf("Section Headers:\n");
    printf("  [Nr] Name              Type             Address           Offset\n");
    printf("       Size              EntSize          Flags  Link  Info  Align\n");

    for (int i = 0; i < ehdr->e_shnum; ++i) 
    {
        printf("  [%2d] %-16s  %-12s  0x%016lx  0x%08lx\n",
               i, 
               get_section_name(i, shdrs, strtab),
               get_section_type(shdrs[i].sh_type), 
               (unsigned long)shdrs[i].sh_addr, 
               (unsigned long)shdrs[i].sh_offset);
        
        printf("       0x%016lx  0x%016lx  %s  %d  %d  %ld\n", 
               (unsigned long)shdrs[i].sh_size, 
               (unsigned long)shdrs[i].sh_entsize, 
               get_section_flags(shdrs[i].sh_flags), 
               shdrs[i].sh_link, 
               shdrs[i].sh_info, 
               shdrs[i].sh_addralign);
    }

    free(strtab);
    free(shdrs);
}


int main(int argc, char *argv[]) 
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <elf-file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd < 0)
    {
        perror("open()");
        return EXIT_FAILURE;
    }

    Elf64_Ehdr ehdr;
    read(fd, &ehdr, sizeof(ehdr));
    print_ehdr(&ehdr);
    print_shdr(fd, &ehdr);
    close(fd);
}
