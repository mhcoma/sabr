#include <stdio.h>

#include "interpreter.h"

int main(int argc, char* argv[]) {
	interpreter inter;
	if (!interpreter_init(&inter)) return 1;

	if (argc == 1) {
		fputs("error : No input files\n", stderr);
		return 2;
	}
	interpreter_load_code(&inter, argv[1]);
	interpreter_run(&inter);

	interpreter_del(&inter);
	return 0;
}