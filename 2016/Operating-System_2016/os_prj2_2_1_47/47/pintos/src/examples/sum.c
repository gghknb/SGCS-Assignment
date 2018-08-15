#include <stdio.h>
#include "userprog/syscall.h"
#include <stdlib.h>
#include <syscall.h>

int main(int arg,char* argv[])
{
    printf("%d %d\n",pibonacci(atoi(argv[1])) ,sum_of_four_integer(atoi(argv[1]),atoi(argv[2]),atoi(argv[3]),atoi(argv[4])));
    return EXIT_SUCCESS;
}
