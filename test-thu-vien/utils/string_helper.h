#ifndef STRING_HELPER_H
#define STRING_HELPER_H

#include <string>
#include "ngay.h"

using namespace std;

class StringHelper {
public:
    static bool isAlpha(char c) {
        return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
    }

    static char toUpper(char c) {
        if (c >= 'a' && c <= 'z') {
            return c - 32;
        }
        return c;
    }

    static string toUpper(string s) {
        string newUpperStr = "";
        for (char c : s) {
            newUpperStr += toUpper(c);
        }

        return newUpperStr;
    }

    static string toString(int n) {
        if (n == 0) return "0";

        bool isNegative = false;
        if (n < 0) {
            isNegative = true;
            n = -n;
        }

        string result = "";
        while (n > 0) {
            char digit = '0' + (n % 10);
            result = digit + result;
            n /= 10;
        }

        if (isNegative) result = "-" + result;

        return result;
    }

    static string toString(Ngay ngay) {
        if (ngay.isDefault()) {
            return "";
        }

        string ngayStr = toString(ngay.getNgay());
        string thangStr = toString(ngay.getThang());
        if (ngay.getThang() < 10) {
            thangStr = "0" + thangStr;
        }

        string namStr = toString(ngay.getNam());

        return ngayStr + "/" + thangStr + "/" + namStr;
    }
};

#endif // STRING_HELPER_H
