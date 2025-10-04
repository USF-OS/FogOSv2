#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "time.h"

static const char* month_name[] = {
    "Jan", "Feb", "Mar", "Apr", "May", "June",
    "July", "Aug", "Sep", "Oct", "Nov", "Dec"
};

int main() {
    uint64 c = clock() / 1000000000ULL;
    uint64 local_date = c - (7 * 3600);
    datetime date = to_datetime(local_date);
    
    char hour[3], minute[3], second[3];

    if (date.hour < 10) {
        hour[0] = '0';
        hour[1] = date.hour + '0';
        hour[2] = '\0';
    } else {
        hour[0] = (date.hour / 10) + '0';
        hour[1] = (date.hour % 10) + '0';
        hour[2] = '\0';
    }

    if (date.minute < 10) {
        minute[0] = '0';
        minute[1] = date.minute + '0';
        minute[2] = '\0';
    } else {
        minute[0] = (date.minute / 10) + '0';
        minute[1] = (date.minute % 10) + '0';
        minute[2] = '\0';
    }

    if (date.second < 10) {
        second[0] = '0';
        second[1] = date.minute + '0';
        second[2] = '\0';
    } else {
        second[0] = (date.second / 10) + '0';
        second[1] = (date.second % 10) + '0';
        second[2] = '\0';
    }

    printf("%s %d %s:%s:%s PDT %d\n", 
    month_name[date.month-1], date.day, 
    hour, minute, second, date.year);
    return 0;
}
