#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "user/uthread.h"

struct thread all_thread[MAX_THREAD];
struct thread *current_thread;
extern void thread_switch(struct context*, struct context*);

// Forward declarations
void thread_schedule(void);

void 
thread_init(void)
{
  // Main thread is thread 0
  current_thread = &all_thread[0];
  current_thread->state = RUNNING;
  
  // Mark other threads as FREE
  for(int i = 1; i < MAX_THREAD; i++) {
    all_thread[i].state = FREE;
  }
}

void 
thread_schedule(void)
{
  struct thread *next = 0;
  struct thread *cur = current_thread;

  // Find next runnable thread (round-robin)
  int start_index = (cur - all_thread + 1) % MAX_THREAD;
  for(int i = 0; i < MAX_THREAD; i++) {
    int index = (start_index + i) % MAX_THREAD;
    if(all_thread[index].state == RUNNABLE) {
      next = &all_thread[index];
      break;
    }
  }

  if(next == 0) {
    // No runnable threads found
    printf("thread_schedule: no runnable threads\n"); exit(0);
    return;
  }

  if(cur != next) {
    next->state = RUNNING;
    struct thread *prev_thread = cur;
    current_thread = next;
    
    thread_switch(&prev_thread->context, &next->context);
  }
}

void 
thread_create(void (*func)())
{
  struct thread *t;

  // Find free thread (skip thread 0 which is main)
  for(t = all_thread + 1; t < all_thread + MAX_THREAD; t++) {
    if(t->state == FREE) {
      break;
    }
  }

  if(t == all_thread + MAX_THREAD) {
    printf("thread_create: no free threads\n");exit(0);
    return;
  }

  t->state = RUNNABLE;

  // Simple stack setup
  t->context.sp = (uint64)t->stack + STACK_SIZE;
  t->context.ra = (uint64)func;
}

void 
thread_yield(void)
{
  current_thread->state = RUNNABLE;
  thread_schedule();
}

volatile int a_started, b_started, c_started;
volatile int a_n, b_n, c_n;

void 
thread_a(void)
{
  int i;
  printf("thread_a_started\n");
  a_started = 1;
  
  while(b_started == 0 || c_started == 0)
    thread_yield();

  for(i = 0; i < 100; i++) {
    printf("thread_a_%d\n", i);
    a_n += 1;
    thread_yield();
  }
  
  printf("thread_a: exit after 100\n");
  
  // Set state to FREE and yield ONE LAST TIME to let other threads run
  current_thread->state = FREE;
  thread_schedule();
  
  // Should never reach here
  while(1) {}
}

void 
thread_b(void)
{
  int i;
  printf("thread_b_started\n");
  b_started = 1;
  
  while(a_started == 0 || c_started == 0)
    thread_yield();

  for(i = 0; i < 100; i++) {
    printf("thread_b_%d\n", i);
    b_n += 1;
    thread_yield();
  }
  
  printf("thread_b: exit after 100\n");
  current_thread->state = FREE;
  thread_schedule();
  
  while(1) {}
}

void 
thread_c(void)
{
  int i;
  printf("thread_c_started\n");
  c_started = 1;
  
  while(a_started == 0 || b_started == 0)
    thread_yield();

  for(i = 0; i < 100; i++) {
    printf("thread_c_%d\n", i);
    c_n += 1;
    thread_yield();
  }
  
  printf("thread_c: exit after 100\n");
  current_thread->state = FREE;
  thread_schedule();
  
  while(1) {}
}

int 
main(int argc, char *argv[]) 
{
  a_started = b_started = c_started = 0;
  a_n = b_n = c_n = 0;
  
  thread_init();
  thread_create(thread_a);
  thread_create(thread_b);
  thread_create(thread_c);
  
  thread_schedule();
  exit(0);
}
