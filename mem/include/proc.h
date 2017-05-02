#ifndef PROC_H
#define PROC_H

#ifdef __ASSEMBLER__

#define SAVE_CONTEXT\
  push {r0, r14};\
  mov r0, lr;\
  // bl save_context_1;\
  pop {r0, r14};\
  push {r14}\
  // bl save_context_2;\
  pop {r14};

#else

#include <vm.h>
#include <types.h>


typedef void (*entry_function)(void);
enum procstate {UNUSED, CREATED, SLEEPING, READY, RUNNING, TERMINATED};

struct proc {
  uint_32 heap_size;
  uint_32 pid;
  entry_function entry;

