#ifndef TOAD_API_H
#define TOAD_API_H

#include "vm.h"
#include "defs.h"

// API functions
void api_putpixel(VM *vm, int x, int y, int color);
double api_cos(double angle);
double api_sin(double angle);
void api_cls(VM *vm);
double api_time(VM *vm);
double api_sqrt(double number);

#endif