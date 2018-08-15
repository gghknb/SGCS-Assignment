#include "userprog/syscall.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "lib/user/syscall.h"
#include "devices/shutdown.h"
#include "devices/input.h"
#include <stdint.h>
#include "userprog/process.h"

static void syscall_handler (struct intr_frame *);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f UNUSED) 
{ 
  int *esp = f->esp;
  switch(*esp)
  {
      case SYS_HALT: syscall_halt(); break;
      case SYS_EXIT: syscall_exit(esp[1]); break;
      case SYS_WAIT: f->eax = syscall_wait((tid_t)esp[1]); break;
      case SYS_EXEC: f->eax = syscall_exec((const char*)esp[1]); break;
      case SYS_READ: f->eax = syscall_read((esp[5]),(void*)(esp[6]),(esp[7])); break;
      case SYS_WRITE: f->eax = syscall_write((esp[5]),(const void*)(esp[6]),(esp[7])); break;
      case SYS_SUM: f->eax = sum_of_four_integer((int)esp[6],(int)esp[7],(int)esp[8],(int)esp[9]); break;
      case SYS_PIBONACCI:  f->eax = pibonacci(esp[1]); break;
      default: break;
  }

}

int sum_of_four_integer(int a,int b,int c,int d)
{
    return a+b+c+d;
}

int pibonacci(int n)
{
    int i;
    int fibo0= 1,fibo1 =1;
    int fibon=0;
    if(n == 1)
        return 1;
    else if(n == 2)
        return 1;
    else
    {
        for( i = 0 ; i < n-2 ; i++)
        {
              fibon = fibo0 +fibo1;
              fibo0 = fibo1;
              fibo1 = fibon;
        }
        return fibon;
    }
}
void syscall_halt()
{
    shutdown_power_off();
}

void syscall_exit(int status)
{
    struct thread* cur_thr = thread_current(); //get current thread
    char *name;
    char *rest;
    
    
    if(status < 0) status = -1;
    if(cur_thr->parent_thr->status == THREAD_BLOCKED)
    {
        thread_unblock(cur_thr->parent_thr);
        cur_thr->parent_thr->status = THREAD_RUNNING;
        cur_thr->parent_thr = NULL;
    }
    cur_thr->exit_status = status;

    name = strtok_r(cur_thr->name," ",&rest);
    printf("%s: exit(%d)\n",name,status);
    thread_exit();
}

pid_t syscall_exec(const char *cmd_line)
{
    return process_execute(cmd_line);
}

int syscall_wait(int pid)
{
    return process_wait((tid_t)(pid));
}

int syscall_read(int fd,void *buffer,unsigned size)
{
    int i;
    int result = 0;
    uint8_t input;
    if(fd == 0)
    {
        for(i = 0 ; i < (int)size ; i++)
        {
            input = input_getc();
            if(input)
            {
                *(uint8_t*)(buffer+i) = input;
                result++;
            }
        }
    }
    else result = 0;

    return result;
}

int syscall_write(int fd,const void *buffer,unsigned size)
{   
    if(fd == 1)
    {
        putbuf((const char*)buffer,size);
    }
    return size;
}
