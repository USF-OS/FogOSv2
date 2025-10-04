#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "time.h"

int is_leap(int year) {
    return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
}

static const int days_per_month[] = {
    31, 28, 31, 30, 31, 30, 
    31, 31, 30, 31, 30, 31
};

datetime to_datetime(uint64 timestamp) {
    datetime date;
    // 86400 seconds in a day
    uint64 secs = timestamp % 86400; // seconds passed today
    uint64 days = timestamp / 86400; // number of days since Jan 1, 1970

    date.hour = secs / 3600; // gets the hour
    if (date.hour > 12) {
        date.hour -= 12; // i don't like military time.
     }
    date.minute = (secs % 3600) / 60; // remaining minutes
    date.second = secs % 60; // remaining seconds

    // unix time starts in 1970
    date.year = 1970;

    while (1) {
        int days_per_yr = is_leap(date.year) ? 366 : 365;
        if (days >= days_per_yr) {
             // subtract days until it fits within a year
            days -= days_per_yr; 
            date.year++;
        } else {
            break;
        }
    }

    int month = 0;
    while (1) {
        int m_days = days_per_month[month];
        if (month == 1 && is_leap(date.year)) {
            m_days = 29;
        }
        if (days >= m_days) {
            // subtract days until it fits in a month
            days -= m_days;
            month++;
        } else {
            break;
        }
    }
    date.month = month + 1; // corrects month 
    date.day = days + 1; // corrects day

    return date;
}

