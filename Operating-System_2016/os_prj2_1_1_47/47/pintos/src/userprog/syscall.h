#include "lib/user/syscall.h"
#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H

void syscall_init (void);

int sum_of_four_integer(int a,int b,int c,int d);
int pibonacci(int n);
void syscall_halt(void);
void syscall_exit(int status);
int syscall_exec(const char *cmd_line);
int syscall_wait(int pid);
int syscall_read(int fd,void *buffer,unsigned size);
int syscall_write(int fd,const void *buffer,unsigned size);

#endif /* userprog/syscall.h */
