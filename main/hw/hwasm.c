#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void handle_hexadecimal_value(char* value, unsigned char* result) {
    char* endPtr;
    char* startPtr = value + 1;
    long int hexValue = strtol(startPtr, &endPtr, 16);
    *result = (unsigned char)hexValue;
}

void handle_char_value(char* value, unsigned char* result) {
    *result = (unsigned char)value[1];
}

unsigned char get_register_mov_opcode(char* _register) {
    if (strcmp(_register, "%al") == 0) {
        return 176;
    } else if (strcmp(_register, "%ah") == 0) {
        return 180;
    }
    return 0;
}

void mov_instruction(char** instructions, int index, FILE* output_file) {
    char* value = strtok(NULL, ",");
    char* _register = strtok(NULL, " ");

    if (_register[0] == '\'') {
        value = "' ',";
        _register = instructions[index+3];
    }

    char prefix = value[0];
    unsigned char result;
    if (prefix == '$') {
        handle_hexadecimal_value(value, &result);
    }
    if (prefix == '\'') {
        handle_char_value(value, &result);
    }

    unsigned char opcode = get_register_mov_opcode(_register);

    fwrite(&opcode, sizeof(unsigned char), 1, output_file);
    fwrite(&result, sizeof(unsigned char), 1, output_file);
}

void int_instruction(char** instructions, int index, FILE* output_file) {
    char* value = strtok(NULL, " ");
    unsigned char result;
    handle_hexadecimal_value(value, &result);
    unsigned char opcode = 205;

    fwrite(&opcode, sizeof(unsigned char), 1, output_file);
    fwrite(&result, sizeof(unsigned char), 1, output_file);
}

void jmp_instruction(FILE* output_file, char * label) {
    unsigned char bytes[] = {235, 253};
    fwrite(bytes, sizeof(unsigned char), 2, output_file);
}

void hlt_instruction(FILE* output_file) {
    unsigned char byte = 244;
    fwrite(&byte, sizeof(unsigned char), 1, output_file);
}

void word_instruction(char** instructions, int index, FILE* output_file) {
    char* value = strtok(NULL, " ");
    char* endPtr;
    long int hexValue = strtol(value, &endPtr, 16);
    
    fwrite(&hexValue, sizeof(short int), 1, output_file);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: %s <input_filename> <output_filename>\n", argv[0]);
        return 1;
    }

    FILE* input_file = fopen(argv[1], "r");
    FILE* output_file = fopen(argv[2], "wb");
    if (!input_file || !output_file) {
        printf("Error opening files.\n");
        return 1;
    }

    char line[1000];
    while (fgets(line, sizeof(line), input_file)) {
        line[strcspn(line, "\n")] = 0;

        char* token;
        token = strtok(line, " ");
        while (token != NULL) {
            if (strcmp(token, "#") == 0) {
                break;
            } else if (strcmp(token, "mov") == 0) {
                mov_instruction(&token, 0, output_file);
            } else if (strcmp(token, "int") == 0) {
                int_instruction(&token, 0, output_file);
            } else if (strcmp(token, "hlt") == 0) {
                hlt_instruction(output_file);
            } else if (strcmp(token, "jmp") == 0) {
                char* label = strtok(NULL, " ");
                jmp_instruction(output_file, label);
            } else if (strcmp(token, ".fill") == 0) {
                unsigned char zeroBytes[461];
                memset(zeroBytes, 0, sizeof(zeroBytes));
                fwrite(zeroBytes, sizeof(unsigned char), sizeof(zeroBytes), output_file);
            } else if (strcmp(token, ".word") == 0) {
                word_instruction(&token, 0, output_file);
            }
            token = strtok(NULL, " ");
        }
    }

    fclose(input_file);
    fclose(output_file);

    return 0;
}