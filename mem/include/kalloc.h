#ifndef KALLOC_H
#define KALLOC_H

#include <types.h>

struct pglist {
  struct pglist * next;
};

void kalloc_init(uint32_t start, uint32_t end);
