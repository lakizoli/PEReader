// testhw.cpp : Defines the entry point for the console application.
//

#include <stdio.h>

extern "C" __declspec(dllexport) int main (int argc, char *argv[]) {
	printf ("Hello World! argc: %d\n", argc);
	for (int i = 0; i < argc; ++i) {
		printf ("argv[%d]: %s\n", i, argv[i]);
	}
	printf ("\n");
    return 0;
}

