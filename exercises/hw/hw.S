#    SPDX-FileCopyrightText: 2024 Otto Fernandes <ottofernandes2012@gmail.com>
#   
#    SPDX-License-Identifier: GPL-3.0-or-later

# Boot, print hello world on the screen and end program

# Used most of $SYSEG/eg/hw/eg-03.S to write this program



    .code16

    .global _start

_start:

    mov $0xe, %ah       # Set BIOS teletype mode

    mov 'H', %al       # Load 'H' ascii mode
    int $0x10           # Issue BIOS interrupt

    mov 'e', %al       # Load 'e' ascii mode
    int $0x10           # Issue BIOS interrupt

    mov 'l', %al       # Load 'l' ascii mode
    int $0x10           # Issue BIOS interrupt

    mov 'l', %al       # Load 'l' ascii mode
    int $0x10           # Issue BIOS interrupt

    mov 'o', %al       # Load 'o' ascii mode
    int $0x10           # Issue BIOS interrupt


    mov ' ', %al       # Load ' ' ascii mode
    int $0x10           # Issue BIOS interrupt


    mov 'W', %al       # Load 'W' ascii mode
    int $0x10           # Issue BIOS interrupt

    mov 'o', %al       # Load 'o' ascii mode
    int $0x10           # Issue BIOS interrupt

    mov 'r', %al       # Load 'r' ascii mode
    int $0x10           # Issue BIOS interrupt

    mov 'l', %al       # Load 'l' ascii mode
    int $0x10           # Issue BIOS interrupt

    mov 'd', %al       # Load 'd' ascii mode
    int $0x10           # Issue BIOS interrupt

halt:
    hlt                 # Halt the machine
    jmp halt            # Safeguard

    .fill 510 - (.-_start), 1, 0
    .word 0xaa55