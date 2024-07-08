/*
 *    SPDX-FileCopyrightText: 2024 Monaco F. J. <monaco@usp.br>
 *    SPDX-FileCopyrightText: 2024 Otto Fernandes <ottofernandes2012@gmail.com>
 *   
 *    SPDX-License-Identifier: GPL-3.0-or-later
 *
 *  This file is a derivative work from SYSeg (https://gitlab.com/monaco/syseg)
 *  and contains modifications carried out by the following author(s):
 *  Otto Fernandes <ottofernandes2012@gmail.com>
 */

#include "bios2.h"		/* For udelay().      */
#include "kaux.h"		/* For ROWS and COLS. */

/* Video RAM as 2D matrix: short vram[row][col]. */

short (*vram)[COLS] = (short (*)[COLS])0xb8000;

char character_color = 0x02;	/* Default fore/background character color.*/

/* Write 'string' starting at the position given by 'row' and 'col'.
   Text is wrapped around both horizontally and vertically. 

   The implementation manipulates the video-RAM rather than BIOS services.
*/

void writexy(unsigned char row, unsigned char col, const char* string)
{
  int k=0;
  
  while (string[k])
    {

      col = col % COLS;
      row = row % ROWS;
      
      vram[row][col] = color_char(string[k]);
      col++;
      k++;
    }
}

/* Clear the entire screen

   The implementation manipulates the video-RAM rather than BIOS services.

 */

void clearxy()
{
  int i,j;

  for (j=0; j<ROWS; j++)
    for (i=0; i<COLS; i++)
      vram[j][i] = color_char(' ');
}

/* A not-that-impressive splash screen that is entirely superfluous. */

extern const char logo[];
void splash(void)
{
  int i,j, k;

  clearxy();

  for (i=0; i<COLS; i++)
    {
      for (j=0; j<ROWS; j+=2)
	{
	  vram[j][i] = color_char(logo[j*COLS+i]);
	  vram[j+1][COLS-i] = color_char(logo[(j+1)*COLS+(COLS-i)]);
	  udelay (1);
	}
    }

  udelay (500);
  clearxy();
}

void call_program(void * program_addr) {
    __asm__ volatile(
      "  call get_return_addr_into_ebx \n"  // coloca o return address em ebx

      "original_return_addr: \n"  // será usado para calcular o valor que deve ser adicionado à stack

      "  push %%ebx \n"  // colocar o ebx na stack

      "  jmp *%[progAddr] \n"  // jump pra main

      "get_return_addr_into_ebx: \n"
      "  mov (%%esp), %%ebx \n"  // coloca o topo da stack em ebx

      "  mov $prog_finish, %%ecx \n"           // ecx = endereço de prog_finish
      "  sub $original_return_addr, %%ecx \n"  // ecx -= endereço de original_return_addr

      "  add %%ecx, %%ebx \n"  // soma ecx em ebx, ou seja, faz com que ebx aponte para prog_finish
      "  ret \n"

      "prog_finish:"

      ::[progAddr] "r"(program_addr));
}
