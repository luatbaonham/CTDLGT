#ifndef SACH_H
#define SACH_H

#include <string>
#include "enums.h"

using namespace std;

class Sach {
private:
    string maSach;
    TrangThaiSach trangThai;
    string viTri;
public:
    Sach() {
        maSach = "";
        trangThai = TrangThaiSach::CHO_MUON_DUOC;
        viTri = "";
    }

    Sach(string viTri) {
        this->maSach = "";
        this->viTri = viTri;
        this->trangThai = TrangThaiSach::CHO_MUON_DUOC;
    }

    string getMaSach() { return maSach; }
    TrangThaiSach getTrangThai() { return trangThai; }
    string getViTri() { return viTri; }

    void setMaSach(string maSach) { this->maSach = maSach; }
    void setTrangThai(TrangThaiSach trangThai) { this->trangThai = trangThai; }
    void setViTri(string viTri) { this->viTri = viTri; }
};

/// DS lien ket don
class NodeSach {
private:
    Sach data;
    NodeSach *next;
public:
    NodeSach() { next = nullptr; }

    NodeSach(Sach data) {
        this->data = data;
        this->next = nullptr;
    }

    Sach &getData() { return data; }
    NodeSach *getNext() { return next; }

    void setNext(NodeSach *next) { this->next = next; }
};

#endif // SACH_H
