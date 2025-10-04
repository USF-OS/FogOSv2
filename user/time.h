#ifndef TIME_H
#define TIME_H

#include "kernel/types.h"

typedef struct datetime {
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
} datetime;

int is_leap(int year);
datetime to_datetime(uint64 timestamp);

#endif
