#include "userprog/syscall.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "devices/shutdown.h"
#include "lib/kernel/console.h"
#include "devices/input.h"
#include "threads/vaddr.h"
#include "process.h"
#include "filesys/filesys.h"

static void syscall_handler (struct intr_frame *);
struct lock filesys_lock;
struct file_descriptor_info* find_fd_p(int fd);

void
syscall_init (void) 
{
	intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
	lock_init(&filesys_lock);
}

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
	void *esp_t = f->esp;
	int sys_flag = 0;

	switch (*(int*)esp_t) {
		case SYS_HALT:	
			syscall_halt(); break;
		case SYS_EXIT:	
			if (is_user_vaddr(esp_t + 1) == false) syscall_exit(-1);
			else syscall_exit(*((int*)esp_t + 1));
			break;
		case SYS_EXEC:
			if (is_user_vaddr(esp_t + 1) == false) syscall_exit(-1);
			else f->eax = syscall_exec((const char*)(*((int*)esp_t + 1)));
			break;
		case SYS_WAIT:
			if (is_user_vaddr(esp_t + 1) == false) syscall_exit(-1);
			else f->eax = syscall_wait(*((int*)esp_t + 1));
			break;
		case SYS_READ:
			if (is_user_vaddr(esp_t + 7) == false) syscall_exit(-1);
			else {
				f->eax = syscall_read(*((int*)esp_t + 5), (void *)*((int *)esp_t + 6), *((unsigned *)esp_t + 7));
			}
			break;
		case SYS_WRITE:
			if (is_user_vaddr(esp_t + 7) == false) syscall_exit(-1);
			else {
				f->eax = syscall_write(*((int *)esp_t + 5), (void *)*((int *)esp_t + 6), *((unsigned *)esp_t + 7));
			}
			break;
		case SYS_CREATE:
			if (is_user_vaddr(esp_t + 5) == false) syscall_exit(-1);
			else f->eax = syscall_create((const char*)(*((int*)esp_t + 4)), *((unsigned *)esp_t + 5));
			break;
		case SYS_REMOVE:
			if (is_user_vaddr(esp_t + 1) == false) syscall_exit(-1);
			else f->eax = syscall_remove((const char*)(*((int*)esp_t + 1)));
			break;
		case SYS_OPEN:
			if (is_user_vaddr(esp_t + 1) == false) syscall_exit(-1);
			else f->eax = syscall_open((const char*)(*((int*)esp_t + 1)));
			break;
		case SYS_FILESIZE:
			if (is_user_vaddr(esp_t + 1) == false) syscall_exit(-1);
			else f->eax = syscall_filesize(*((int*)esp_t + 1));
			break;
		case SYS_SEEK:
			if (is_user_vaddr(esp_t + 5) == false) syscall_exit(-1);
			else syscall_seek(*((int*)esp_t + 4), *((unsigned*)esp_t + 5));
			break;
		case SYS_TELL:
			if (is_user_vaddr(esp_t + 1) == false) syscall_exit(-1);
			else f->eax = syscall_tell(*((int*)esp_t + 1));
			break;
		case SYS_CLOSE:
			if (is_user_vaddr(esp_t + 1) == false) syscall_exit(-1);
			else syscall_close(*((int*)esp_t + 1));
			break;
		case SYS_SUM:
			if (is_user_vaddr(esp_t + 9) == false) syscall_exit(-1);
			else f->eax = sum_of_four_integer(*((int*)esp_t + 6), *((int*)esp_t + 7), *((int*)esp_t + 8), *((int*)esp_t + 9));
			break;
		case SYS_PIBONACCI:
			if (is_user_vaddr(esp_t + 1) == false) syscall_exit(-1);
			else f->eax = pibonacci(*((int*)esp_t + 1));
			break;
		default:
			syscall_exit(-1); break;
	}

	return;
}

void syscall_halt()
{
	shutdown_power_off(); 
	return;
}

int syscall_exec(const char *cmd_line)
{
	return process_execute(cmd_line);
}

int syscall_wait(int pid)
{
	return process_wait(pid);
}

void syscall_exit(int status)
{
	struct thread *cur = thread_current();
	struct thread *child_t;
    struct list_elem *e;
	char *token;
    char *rest;
    struct file_descriptor_info *temp;
	
	if(status <= -1)
		status = -1;
    cur->exist_status = status;
	token = strtok_r(cur->name," \n",&rest);
	printf("%s: exit(%d)\n",token,status);
	
	if (is_file == true) {
		is_file = false;
		thread_check = false;
	}

    for(;;)
    {
        if(list_empty(&cur->fd_list) == true)
            break;
        e = list_pop_back(&cur->fd_list);
		temp = list_entry(e, struct file_descriptor_info, fd_list_elem);
		file_close(temp->file_p);
		free(temp);
    }
    for(;;)
    {
        if(list_empty(&cur->child_list) == true)
            break;
        e = list_pop_back(&cur->child_list);
    }
    /*
	while(list_empty(&cur->fd_list) == false)
	{
		e = list_pop_back(&cur->fd_list);
		temp = list_entry(e, struct file_descriptor_info, fd_list_elem);
		file_close(temp->file_p);
		free(temp);
	}

	while(list_empty(&cur->child_list) == false)
		e = list_pop_back(&cur->child_list);
    */
	thread_exit();                                  
	return;
}

int syscall_read(int fd, void *buffer, unsigned size)
{
	int result = 0;
	unsigned i;
	uint8_t input;
	struct file_descriptor_info *temp = NULL;

	if (is_user_vaddr(buffer) == false)
		syscall_exit(-1);

	lock_acquire(&filesys_lock);
	if (fd == 0)
	{
		result= 0;
		for (i = 0; i < size; i++)
		{
		    input = input_getc();
			*(uint8_t*)buffer = input;
			result += 1;
			if (result == size)
				break;
		}
	}
	else if(fd > 1)
	{
		temp = find_fd_p(fd);
		if (temp)
			result = file_read(temp->file_p, buffer, size);
		else
			result = -1;
	}
	lock_release(&filesys_lock);
	return result;
}

int syscall_write(int fd, const void *buffer, unsigned size)
{
	struct file_descriptor_info *fdt = NULL;
	
	if (is_user_vaddr(buffer) == false)
		syscall_exit(-1);

	lock_acquire(&filesys_lock);

	if(fd > 1)
	{
		fdt = find_fd_p(fd);
		if (fdt)
			size = file_write(fdt->file_p, buffer, size);
		else
			size = -1;
	}
    else if(fd == 1)
        putbuf(buffer,size);
	lock_release(&filesys_lock);
	return size;
}

int sum_of_four_integer(int a, int b, int c, int d)
{
	return a + b + c + d;
}

int pibonacci(int n)
{
	int i;
	int fibo0 = 1, fibo1 = 1;
	int fibon = 0;
	if (n == 1)
		return 1;
	else if (n == 2)
		return 1;
	else
	{
		for (i = 0; i < n - 2; i++)
		{
			fibon = fibo0 + fibo1;
			fibo0 = fibo1;
			fibo1 = fibon;
		}
		return fibon;
	}
}

int syscall_create(const char *file, unsigned initial_size)
{
	int result;

	if (file==NULL)
		syscall_exit(-1);

	lock_acquire(&filesys_lock);
	result = filesys_create(file, initial_size);
	lock_release(&filesys_lock);

	return result;
}

int syscall_remove(const char *file)
{
	int result;
    int flag = 0;
	if(file==NULL)
    {
        flag = 1;
		syscall_exit(-1);
    }
    if(flag == 0)
    {
    	lock_acquire(&filesys_lock);
    	result = filesys_remove(file);
    	lock_release(&filesys_lock);
    }
	return result;
}

int syscall_open(const char *file)
{
	struct thread *cur_t = thread_current();
    struct file *fp;
    struct file_descriptor_info *temp;
	unsigned int fd_num;

	if(!file)
		syscall_exit(-1);

	lock_acquire(&filesys_lock);
	fp = filesys_open(file);
    temp = (struct file_descriptor_info *)malloc(sizeof(struct file_descriptor_info));
	if (fp == NULL)
		fd_num = -1;
	else
	{
        cur_t -> fd_num += 1;
		fd_num = cur_t -> fd_num - 1;
		temp->fd = fd_num;
		temp->file_p = fp;
		list_push_back(&(cur_t->fd_list),&(temp->fd_list_elem));
	}

	if (strcmp(file,thread_name()) == 0)
		file_deny_write(fp);

	lock_release(&filesys_lock);

	return fd_num;
}

int syscall_filesize(int fd)
{
	struct file_descriptor_info *temp = find_fd_p(fd);
	struct file *fp;
	int size_flag = 0;
    int result;
    
	if(!temp)
    {
        size_flag = -1;
		return size_flag;
    }

    if(size_flag == 0)
    {
    	lock_acquire(&filesys_lock);
    	fp = temp->file_p;
    	result = file_length(fp);
    	lock_release(&filesys_lock);
    }
	return result;
}

void syscall_seek(int fd, unsigned position)
{
	struct file_descriptor_info *temp = find_fd_p(fd);

	if(!temp)
		syscall_exit(-1);
	else if(temp)
	{	
		lock_acquire(&filesys_lock);
		file_seek(temp->file_p , position);
		lock_release(&filesys_lock);
	}

	return;
}

unsigned syscall_tell(int fd)
{
	struct file_descriptor_info *temp = find_fd_p(fd);
	unsigned ret;
    int place = 0;
	temp = find_fd_p(fd);

	if(!temp)
    {
        place = -1;
		return -1;
    }

    if(place = 0)
    {
	    lock_acquire(&filesys_lock);
    	ret=file_tell(temp->file_p);
    	lock_release(&filesys_lock);
    }
	return ret;
}

void syscall_close(int fd)
{
	struct file_descriptor_info *temp = find_fd_p(fd);

	if(!temp)
		syscall_exit(-1);
	else if(temp)
	{
		lock_acquire(&filesys_lock);
		list_remove(&temp->fd_list_elem);
		file_close(temp->file_p);
		free(temp);
		lock_release(&filesys_lock);
	}
}

struct file_descriptor_info* find_fd_p(int fd)
{
	struct thread *cur_t = thread_current();
	struct list_elem *e;
	struct file_descriptor_info *temp = NULL;
	int flag = 0, temp_fd;

	for (e = list_begin(&cur_t->fd_list); e != list_end(&cur_t->fd_list); e = list_next(e))
	{
		temp_fd = list_entry(e, struct file_descriptor_info, fd_list_elem)->fd;
		if (temp_fd == fd)
		{
			temp = list_entry(e, struct file_descriptor_info, fd_list_elem);
			flag++;break;
		}
	}

	if (flag == 0)
		return NULL;

	return temp;
}
