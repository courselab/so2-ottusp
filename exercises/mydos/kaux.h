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

#ifndef KLIB_H
#define KLIB_H

typedef char byte;

#define COLS    80		/* VGA-compatible text mode: 80 columns. */
#define ROWS    25		/* VGA-compatible text mode: 25 rows.    */
#define UPPER    0		/* First row (upper of the screen).      */
#define BOTTOM  (ROWS-1)	/* Last row (bottom of the screen).      */
#define LEFT    0		/* First column (left of the screen).    */
#define RIGHT   (COLS -1)	/* Last column (right of the screen).    */

#define USER_PROGRAM_START_ADDR 0xFE00

extern char character_color;    /* Default fore/background char color.   */

void splash(void);		/* Draw the splash screen.               */
void halt(void);		/* Halt the system.                       */

/* Two-byte value to be written into video RAM: color+ascii.             */

#define color_char(ascii) ((character_color<<8) + ascii)

int syscall();

void register_syscall_handler();

void input (char *);

void writexy(unsigned char, unsigned char, const char *);
void clearxy(void);

void call_program(void * program_addr);

#endif  /* KLIB_H  */
