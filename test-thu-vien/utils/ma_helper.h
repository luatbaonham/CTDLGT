#ifndef MA_HELPER_H
#define MA_HELPER_H

#include <string>

using namespace std;

class MaHelper {
public:
    static string layISBNTheoMaSach(string maSach) {
        string isbn = "";
        for (int i = maSach.length() - 1; i >= 0; i--) {
            if (maSach[i] == '_') break;
            isbn.insert(0, 1, maSach[i]);
        }

        return maSach.substr(0, maSach.length() - isbn.length() - 1);
    }
};

#endif // MA_HELPER_H
