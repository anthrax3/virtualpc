/*
 * main.c
 *
 *  Created on: Feb 11, 2018
 *      Author: nullifiedcat
 */

#include "array.h"
#include <stdio.h>
#include "compiler.h"

int main(int argc, const char **argv)
{
    if (argc < 2)
    {
        printf("ncas <file>\n");
        return 1;
    }

    struct compiler_state_s *state = compiler_init();

    int result = compiler_process_file(state, argv[1]);
    printf("compiler exited with code %d\n", result);

    compiler_destroy(state);

    return 0;
}
