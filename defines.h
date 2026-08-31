#ifndef DEFINES_H
#define DEFINES_H

#define IS_INT(flags)   !(flags | 0)
#define IS_FLOAT(flags) flags & 1 << 7
#define IS_CHAR(flags)  !(flags & 1 << 7) && flags | 0
#define IS_ERROR(flags) flags & 1 << 6

#define NO_ERROR_CODE       0
#define SINTAX_ERROR_CODE   1
#define DIV_ZERO_ERROR_CODE 2
#define OVERFLOW_ERROR_CODE 3

#endif