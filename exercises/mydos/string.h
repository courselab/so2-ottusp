/*
 *    SPDX-FileCopyrightText: 2024 Otto Fernandes <ottofernandes2012@gmail.com>
 *   
 *    SPDX-License-Identifier: GPL-3.0-or-later
 *
 *    This file is part of SYSeg, available at https://gitlab.com/monaco/syseg.
 */

#ifndef STRING_H
#define STRING_H

void itos(int num, char str[]);

void reverse(char str[], int length);

int strcmp(const char *s1, const char *s2);

#endif  /* STRING_H  */