#include <stdio.h>
#include "tests/threads/tests.h"
#include "threads/init.h"
#include "threads/malloc.h"
#include "threads/synch.h"
#include "threads/thread.h"
#include "devices/timer.h"

static thread_func alarm_priority_thread;
static int64_t wake_time;
static struct semaphore wait_sema;
//BASED ON ALARM-PRIORITY
void
test_n (void)
{
  int i;

  /* This test does not work with the MLFQS. */
  ASSERT (selected_scheduler != SCH_MLFQS);

  wake_time = timer_ticks () + 5 * TIMER_FREQ;
  sema_init (&wait_sema, 0);

  for (i = 0; i < 100; i++)
    {
      int priority = PRI_DEFAULT;
      char name[16];
      thread_create (name, priority, alarm_priority_thread, NULL);
    }

  for (i = 0; i < 100; i++)
    sema_down (&wait_sema);
}

static void
alarm_priority_thread (void *aux UNUSED)
{
  int64_t start_time = timer_ticks ();
  while (timer_elapsed (start_time) == 0)
    continue;
  timer_sleep (wake_time - timer_ticks ());
  msg ("Thread %s woke up.", thread_name ());

  sema_up (&wait_sema);
}
