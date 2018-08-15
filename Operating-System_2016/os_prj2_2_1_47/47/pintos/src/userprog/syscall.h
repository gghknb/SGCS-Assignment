#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H

void syscall_init (void);
void syscall_halt (void);
void syscall_exit (int status);
int syscall_exec (const char *cmd_line);
int syscall_wait(int pid);
int syscall_read(int fd, void *buffer, unsigned size);
int syscall_write(int fd, const void *buffer, unsigned size);
int syscall_create(const char *file, unsigned initial_size);
int syscall_remove(const char *file);
int syscall_open(const char *file);
int syscall_filesize(int fd);
void syscall_seek(int fd, unsigned position);
unsigned syscall_tell(int fd);
void syscall_close(int fd);
int sum_of_four_integer(int a,int b,int c,int d);
int pibonacci(int n);

#endif /* userprog/syscall.h */
