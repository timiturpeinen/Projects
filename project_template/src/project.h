#ifndef _PROJECT__H_
#define _PROJECT__H_

typedef struct calendar_st Calendar;

typedef struct appointment_st{
    char* description;
    int month;
    int day;
    int hour;
} appointment;

struct calendar_st{
    appointment* appointments;
    int count;
};

#endif //! _PROJECT__H_