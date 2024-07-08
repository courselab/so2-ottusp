/*
 *    SPDX-FileCopyrightText: 2021 Monaco F. J. <monaco@usp.br>
 *    SPDX-FileCopyrightText: 2024 Otto Fernandes <ottofernandes2012@gmail.com>
 *   
 *    SPDX-License-Identifier: GPL-3.0-or-later
 *
 *  This file is a derivative work from SYSeg (https://gitlab.com/monaco/syseg)
 *  and contains modifications carried out by the following author(s):
 *  Otto Fernandes <ottofernandes2012@gmail.com>
 */

/* This source file implements the kernel entry function 'kmain' called
   by the bootloader, and the command-line interpreter. Other kernel functions
   were implemented separately in another source file for legibility. */

#include "bios1.h"		/* For kwrite() etc.            */
#include "bios2.h"		/* For kread() etc.             */
#include "kernel.h"		/* Essential kernel functions.  */
#include "kaux.h"		/* Auxiliary kernel functions.  */
#include "file_system.h"
#include "tydos.h"
#include "string.h"

/* Kernel's entry function. */

void kmain(void)
{
  int i, j;
  
  register_syscall_handler();	/* Register syscall handler at int 0x21.*/

  splash();			/* Uncessary spash screen.              */

  shell();			/* Invoke the command-line interpreter. */
  
  halt();			/* On exit, halt.                       */
  
}

/* Tiny Shell (command-line interpreter). */

char buffer[BUFF_SIZE];
int go_on = 1;

void shell()
{
  int i;
  clear();
  kwrite ("LastDanceOS 1.0\n");

  while (go_on)
    {

      /* Read the user input. 
	 Commands are single-word ASCII tokens with no blanks. */
      do
	{
	  kwrite(PROMPT);
	  kread (buffer);
	}
      while (!buffer[0]);

      /* Check for matching built-in commands */
      
      i=0;
      while (cmds[i].funct)
	{
	  if (!strcmp(buffer, cmds[i].name))
	    {
	      cmds[i].funct();
	      break;
	    }
	  i++;
	}

      /* If the user input does not match any built-in command name, just
	 ignore and read the next command. If we were to execute external
	 programs, on the other hand, this is where we would search for a 
	 corresponding file with a matching name in the storage device, 
	 load it and transfer it the execution. Left as exercise. */
      
      if (!cmds[i].funct)
	kwrite ("Command not found\n");
    }
}


/* Array with built-in command names and respective function pointers. 
   Function prototypes are in kernel.h. */

struct cmd_t cmds[] =
  {
    {"help",    f_help},     /* Print a help message.       */
    {"quit",    f_quit},     /* Exit LDOS.                 */
    {"list",    f_list},     /* List files in LDOS          */
    {"exec",    f_exec},     /* Execute an example program. */
    {0, 0}
  };


/* Build-in shell command: help. */

void f_help()
{
  kwrite ("...me, Obi-Wan, you're my only hope!\n\n");
  kwrite ("   But we can try also some commands:\n");
  kwrite ("      exec    (to execute an user program example)\n");
  kwrite ("      list    (to list the files in LDOS)\n");
  kwrite ("      quit    (to exit LDOS)\n");
}

void f_quit()
{
  kwrite ("Program halted. Bye.");
  go_on = 0;
}

void f_list() {
  fs_header *header = get_fs_header();

  int sector_coordinate = 1 + header->number_of_boot_sectors;
  int sectors_to_read = header->number_of_file_entries * DIR_ENTRY_LEN / SECTOR_SIZE;

  extern byte _MEM_POOL;
  void *directory_section = (void *)&_MEM_POOL;

  load_disk_into_memory(sector_coordinate, sectors_to_read, directory_section);

  for (int i = 0; i < header->number_of_file_entries; i++) {
    char *file_name = directory_section + i * DIR_ENTRY_LEN;
    if(file_name[0]) {
      kwrite(file_name);
      kwrite("\n");
    }
  }
}

void kwrite_number(int a) {
  char position[10];
  itos(a, position);
  kwrite(position);
}

void f_exec() {
  kwrite("Program name: ");
  char program_name[10];
  gets(program_name);

  fs_header *header = get_fs_header();

  int boot_sectors = 1 + header->number_of_boot_sectors;
  int file_name_sectors = header->number_of_file_entries * DIR_ENTRY_LEN / SECTOR_SIZE;

  extern byte _MEM_POOL;
  void *directory_section = (void *)&_MEM_POOL;

  load_disk_into_memory(boot_sectors, file_name_sectors, directory_section);

  int bin_page_number = -1;
  for (int i = 0; i < header->number_of_file_entries; i++) {
    char *file_name = directory_section + i * DIR_ENTRY_LEN;

    if(file_name[0] && !strcmp(file_name, program_name)) {
      bin_page_number = i;
      break;
    }
  }

  if (bin_page_number == -1) {
    kwrite("Program not found in disk...\n");
    return;
  }

  void * bin_sector_coordinate = boot_sectors + file_name_sectors + header->max_file_size * bin_page_number - 1;

  int memory_offset = header->number_of_file_entries * DIR_ENTRY_LEN - (file_name_sectors - 1) * 512;
  void * relative_memory_program_sector = (void *) USER_PROGRAM_START_ADDR;
  void * memory_program_sector = relative_memory_program_sector - memory_offset;

  load_disk_into_memory(bin_sector_coordinate, header->max_file_size, memory_program_sector);

  call_program(memory_program_sector);
}

