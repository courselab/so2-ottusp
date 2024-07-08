#include "tydos.h"

int main() {
	puts("What's your name?\n");
	char name[10];
	gets(name);
	puts("Hello ");
	puts(name);
	puts("!\n");

	return 0;
}