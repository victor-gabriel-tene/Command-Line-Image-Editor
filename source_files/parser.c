// Tene Victor-Gabriel, 315CA

#include <stdio.h>
#include <string.h>

#include "parser.h"

int parse_input(char args[][ARGUMENTS_DIM])
{
	// creates a buffer, reads and stores the input inside it
	char buffer[BUFFER_SIZE];
	if (!fgets(buffer, BUFFER_SIZE, stdin)) {
		memset(args, 0, ARGUMENTS_CNT * ARGUMENTS_DIM);
		return 0;
	}
	if (buffer[strlen(buffer) - 1] == '\n') {
		buffer[strlen(buffer) - 1] = '\0';
	}

	// defines the number of arguments resulted from parsing the input
	int arg_cnt = 0;

	// parses the buffer into arguments having <delim> as delimitators
	char delim[] = " \t";
	char *arg = strtok(buffer, delim);
	while (arg) {
		strcpy(args[arg_cnt++], arg);
		arg = strtok(NULL, delim);
	}

	return arg_cnt;
}
