#ifndef DATE_HELPER_H
#define DATE_HELPER_H

#include <ctime>
#include "ngay.h"

class DateHelper {
public:
    static Ngay getCurrentDate() {
        time_t t = time(0);
        tm* now = localtime(&t);
        return Ngay(now->tm_mday, now->tm_mon + 1, now->tm_year + 1900);
    }
};

#endif // DATE_HELPER_H
