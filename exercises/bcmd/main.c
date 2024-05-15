/*
 *    SPDX-FileCopyrightText: 2024 MatheusBViana <matheusbviana@hotmail.com>
 *   
 *    SPDX-License-Identifier: GPL-3.0-or-later
 *
 *  This file is a derivative work from SYSeg (https://gitlab.com/monaco/syseg)
 *  and contains modifications carried out by the following author(s):
 *  MatheusBViana <matheusbviana@hotmail.com>
 */

#include "bios.h"
#include "utils.h"

#define SIZE 20         

char buffer[SIZE];       

int is_palindrome(const char *str) {
    const char *end = str;
    while (*end != '\0') {
        end++;
    }
    end--;
    while (str < end) {
        if (*str != *end) {
            return 0; 
        }
        str++;
        end--;
    }
    return 1;  
}


int main() {
    while (1) {
        readln(buffer);     
        if (strcmp(buffer, "prof")){
            if(is_palindrome(buffer)){
                println("é palindromo");
            }else{
                println("nao eh palindromo");
            }
        }else{
            println("Monaco");
        }
    }

    return 0;
}
