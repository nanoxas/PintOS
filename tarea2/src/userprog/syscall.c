#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include <user/syscall.h>
#include "threads/interrupt.h"
#include "threads/synch.h"
#include "threads/thread.h"
#include "threads/child.h"
#include "userprog/process.h"


// Implementation Parameters
// =========================
static int WRITE_SIZE=128;



inline
int
syscall_write(struct intr_frame *f)
{
	int *p = f->esp;  // Stack pointer (x86 registers http://www.scs.stanford.edu/05au-cs240c/lab/i386/s02_03.htm)

	//FIXME: This is (probably) displaced because of the -12 on process.c:442 (setup_stack)
	//int file_descriptor =         p[1];
	//char        *buffer = (char*) p[2];
	//int            size = (int)   p[3];  // huge size may cause overflow

	int file_descriptor =         p[5];
	char        *buffer = (char*) p[6];
	int            size = (int)   p[7];  // may overflow signed int!!

	switch(file_descriptor)
	{
	case STDIN_FILENO:
		// Inform that no data was written
		f->eax=0;
		// REVIEW: Should that process be terminated?
		return 2;

	case STDOUT_FILENO:
	{
		// Write in chunks of WRITE_SIZE
		//   putbuf (src/lib/kernel/console.c) locks the console,
		//   we should avoid locking it too often or for too long
		int remaining = size;
		while(remaining > WRITE_SIZE)
		{
			// Write a chunk
			putbuf(buffer, WRITE_SIZE);
			// Advance buffer pointer
			buffer    += WRITE_SIZE;
			remaining -= WRITE_SIZE;
		}
		// Write all the remaining data
		putbuf(buffer, remaining);

		// Inform the amount of data written
		f->eax=(int)size;
		return 0;
	}

	default:
		printf("syscall: write call not implemented for files\n");
		return 1;
	}

	return 1;  // Unreachable, but compiler complains
}


void exitThread(int code){
	struct thread* t = thread_current();
	t->exitcode = code;
	printf("thread pid: %d, exit code: %d\n", t->tid, code);
	thread_exit();
}


void halt(){
	shutdown_power_off();
}

int wait(pid_t child_tid){
	printf("SYSCALL WAIT\n");
	return process_wait(child_tid);
}
tid_t exec(const char* cmd_line){

	static char *argv[128 / 2 + 1];
	char *save_ptr;
	int i=1;
	argv[0] = "run";
	argv[1] = strtok_r((char *) cmd_line, " ", &save_ptr);


	while(argv[i] != NULL)
	{
		//printf("--Arg %d: %s\n", i, argv[i]);
		i++;
		argv[i] =  strtok_r (NULL, " ", &save_ptr);
	}

	int j=0;
	for(j=0; argv[j]!= NULL; j++)
	{
		//printf("Arg %d to send: %s\n", j, argv[j]);
	}
	tid_t tid = process_execute(argv[1], argv);
	printf("Ejecute\n");

	//Idea de implementacion tomada de ryantimwilson en github.
	child* c =  get_child(tid);
	if(c!=NULL){
		while(1){

			if(c->loaded==0)
			{
				barrier();
			}
			else{
				break;
			}
		}
		//printf("tid: %d",tid);
		return tid;
	}
	return -1;
}








static void
syscall_handler (struct intr_frame *f)
{
	// intr_frame holds CPU register data
	//   Intel 80386 Reference Programmer's Manual (TL;DR)
	//     http://www.scs.stanford.edu/05au-cs240c/lab/i386/toc.htm
	int *p = f->esp;  // Stack pointer (x86 registers http://www.scs.stanford.edu/05au-cs240c/lab/i386/s02_03.htm)
	int syscall_number = (*p);
	
	switch(syscall_number)
	{
	case SYS_HALT:
		printf("system call: halt\n");
		halt();
		break;
	case SYS_EXIT:
		printf("system call: exit\n");
		exitThread(*((int*)f->esp+1));
		break;
	case SYS_EXEC:
		printf("system call: exec\n");
		exec(*((int*)f->esp+1));
		break;
	case SYS_WAIT:
		printf("system call: wait\n");
		wait(*((int*)f->esp+1));
		break;
	case SYS_WRITE:
		syscall_write(f);
		return;
	default:
		printf("system call: unhandled syscall. Terminating process[%d]\n",
				thread_current()->tid);
		break;
	}

	// Syscall handling failed, terminate the process
	thread_exit ();
}








void
syscall_init (void)
{
	intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

