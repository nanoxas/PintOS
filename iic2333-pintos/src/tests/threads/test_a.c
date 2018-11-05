#include <stdio.h>
#include "tests/threads/tests.h"
#include "threads/init.h"
#include "threads/malloc.h"
#include "threads/synch.h"
#include "threads/thread.h"
#include "devices/timer.h"

int queue_number;
int max_turns_in_queue;
int A;
int N;

static thread_func test_a_thread; 
static int64_t wake_time;
static struct semaphore wait_sema;
//BASED ON ALARM-PRIORITY
void test_a(void)
{	

	wake_time = timer_ticks () + 5 * TIMER_FREQ;
  	sema_init (&wait_sema, 0);
	//queue_number = 3;
	//max_turns_on_queue = 3;
	msg ("Starting argument test.\nA: %d\nN: %d.", A, N);
 	thread_create ("thread ", PRI_DEFAULT , test_a_thread, NULL);

 	int i;
 	for (i=0; i<100; i++)
 	{
 		 int priority = PRI_DEFAULT - (i + 7) % 10 - 1;
      	char name[100];
      	snprintf (name, sizeof name, "priority %d", priority);
 		thread_create (name, priority, test_a_thread, NULL);
 	}
 	sema_down (&wait_sema);
 	while(true)
 	{

 	}
}

static void test_a_thread(void *aux UNUSED)
{	
	//thread_print_stats();
	int64_t start_time = timer_ticks ();
  	while (timer_elapsed (start_time) == 0)
    continue;

  /* Now we know we're at the very beginning of a timer tick, so
     we can call timer_sleep() without worrying about races
     between checking the time and a timer interrupt. */
  timer_sleep (wake_time - timer_ticks ());

  /* Print a message on wake-up. */
  msg ("Thread %s woke up.", thread_name ());

  sema_up (&wait_sema);
  int i;
	for(i=0;i<100000000;i++)
	{

	}
	//Print total time on queue
	//Print total quantums used
	//Print number of times that thread changes queues
	//Print number of times thread was chosen

}


