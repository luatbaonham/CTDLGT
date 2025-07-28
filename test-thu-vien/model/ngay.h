#ifndef NGAY_H
#define NGAY_H

#include <cmath>

class Ngay {
private:
    int ngay, thang, nam;

    bool isLeap(int y) {
        return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
    }

    int demNgayTu010101() {
        int ngayTrongThang[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

        int totalDays = ngay;

        for (int y = 1; y < nam; y++) {
            totalDays += isLeap(y) ? 366 : 365;
        }

        for (int m = 1; m < thang; m++) {
            if (m == 2 && isLeap(nam)) totalDays += 29;
            else totalDays += ngayTrongThang[m];
        }

        return totalDays;
    }

public:
    Ngay() {
        ngay = thang = nam = 0;
    }

    Ngay(int ngay, int thang, int nam) {
        this->ngay = ngay;
        this->thang = thang;
        this->nam = nam;
    }

    int getNgay() { return ngay; }
    int getThang() { return thang; }
    int getNam() { return nam; }

    int daysTo(Ngay other) {
        return abs(other.demNgayTu010101() - demNgayTu010101());
    }

    bool isDefault() {
        return ngay == 0 || thang == 0 || nam == 0;
    }
};

#endif // NGAY_H
