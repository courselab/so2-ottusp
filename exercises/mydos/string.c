/*
 *    SPDX-FileCopyrightText: 2024 Otto Fernandes <ottofernandes2012@gmail.com>
 *   
 *    SPDX-License-Identifier: GPL-3.0-or-later
 *
 *    This file is part of SYSeg, available at https://gitlab.com/monaco/syseg.
 */

#include "string.h"

void reverse(char str[], int length) {
    int start = 0;
    int end = length - 1;
    while (start < end) {
        // Swap characters
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

void itos(int num, char str[]) {
    int i = 0;
    int isNegative = 0;

    // Handle 0 explicitly
    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return;
    }

    // Handle negative numbers
    if (num < 0) {
        isNegative = 1;
        num = -num;
    }

    // Process each digit
    while (num != 0) {
        int rem = num % 10;
        str[i++] = (char)(rem + '0');
        num = num / 10;
    }

    // If the number was negative, add the negative sign
    if (isNegative) {
        str[i++] = '-';
    }

    // Append null terminator
    str[i] = '\0';

    // Reverse the string to get the correct result
    reverse(str, i);
}

/* Return 0 is string 's1' and 's2' are equal; return non-zero otherwise.*/

int strcmp(const char *s1, const char *s2) {
  while (*s1 && *s2 && *s1 == *s2) {
    s1++;
    s2++;
  }
  return (*s1 - *s2);
}
