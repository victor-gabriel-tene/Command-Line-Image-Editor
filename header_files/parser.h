// Tene Victor-Gabriel, 315CA

#pragma once

#define BUFFER_SIZE 1000
#define ARGUMENTS_DIM 100
#define ARGUMENTS_CNT 10

// parses each command and its parameters read from stdin
// it returns the number of arguments found, which are stored in <args>
// the command and each parameter are considered arguments
int parse_input(char args[][ARGUMENTS_DIM]);
