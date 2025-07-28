#ifndef MUONTRA_H
#define MUONTRA_H

#include <string>
#include "ngay.h"
#include "enums.h"
#include "date_helper.h"

using namespace std;

class MuonTra {
private:
    string maSach;
    Ngay ngayMuon;
    Ngay ngayTra;
    TrangThaiMuonTra trangThai;
public:
    MuonTra() {
        maSach = "";
        ngayMuon = DateHelper::getCurrentDate(),
        trangThai = TrangThaiMuonTra::CHUA_TRA;
    }

    MuonTra(string maSach) {
        this->maSach = maSach;
        this->ngayMuon = DateHelper::getCurrentDate();
        this->trangThai = TrangThaiMuonTra::CHUA_TRA;
    }

    string getMaSach() { return maSach; }
    Ngay getNgayMuon() { return ngayMuon; }
    Ngay getNgayTra() { return ngayTra; }
    TrangThaiMuonTra getTrangThai() { return trangThai; }

    void setMaSach(string maSach) { this->maSach = maSach; }
    void setNgayMuon(Ngay ngayMuon) { this->ngayMuon = ngayMuon; }
    void setNgayTra(Ngay ngayTra) { this->ngayTra = ngayTra; }
    void setTrangThai(TrangThaiMuonTra trangThai) { this->trangThai = trangThai; }
};

// DS lien ket don
class NodeMuonTra {
private:
    MuonTra data;
    NodeMuonTra *next;
public:
    NodeMuonTra(MuonTra data) {
        this->data = data;
        this->next = nullptr;
    }

    MuonTra &getData() { return data; }
    NodeMuonTra *getNext() { return next; }

    void setNext(NodeMuonTra *next) { this->next = next; }
};

#endif // MUONTRA_H
