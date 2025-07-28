#ifndef DAUSACH_H
#define DAUSACH_H

#include <string>
#include "sach.h"
#include "string_helper.h"

using namespace std;

class DauSach {
private:
    string ISBN;
    string tenSach;
    int soTrang;
    string tacGia;
    int namXuatBan;
    string theLoai;
    int soLanMuon;
    NodeSach *nodeSach;

public:
    DauSach() {
        ISBN = "";
        tenSach = "";
        soTrang = 0;
        tacGia = "";
        namXuatBan = 0;
        theLoai = "";
        soLanMuon = 0;
        nodeSach = nullptr;
    }

    DauSach(string ISBN, string tenSach, int soTrang, string tacGia, int namXuatBan, string theLoai) {
        this->ISBN = ISBN;
        this->tenSach = tenSach;
        this->soTrang = soTrang;
        this->tacGia = tacGia;
        this->namXuatBan = namXuatBan;
        this->theLoai = theLoai;
        this->soLanMuon = 0;
        this->nodeSach = nullptr;
    }

    string getISBN() { return ISBN; }
    string getTenSach() { return tenSach; }
    int getSoTrang() { return soTrang; }
    string getTacGia() { return tacGia; }
    int getNamXuatBan() { return namXuatBan; }
    string getTheLoai() { return theLoai; }
    int getSoLanMuon() { return soLanMuon; }
    NodeSach *getNodeSach() { return nodeSach; }

    void setISBN(string ISBN) { this->ISBN = ISBN; }
    void setTenSach(string tenSach) { this->tenSach = tenSach; }
    void setSoTrang(int soTrang) { this->soTrang = soTrang; }
    void setTacGia(string tacGia) { this->tacGia = tacGia; }
    void setNamXuatBan(int namXuatBan) { this->namXuatBan = namXuatBan; }
    void setTheLoai(string theLoai) { this->theLoai = theLoai; }
    void setSoLanMuon(int soLanMuon) { this->soLanMuon = soLanMuon; }
    void setNodeSach(NodeSach *nodeSach) { this->nodeSach = nodeSach; }

    bool themSach(Sach sach) {
        int viTri = 0;
        NodeSach* current = nodeSach;

        // Nếu danh sách rỗng
        if (current == nullptr) {
            sach.setMaSach(ISBN + "_" + StringHelper::toString(viTri));
            nodeSach = new NodeSach(sach);
            return true;
        }

        // Duyệt đến cuối và đếm số lượng
        while (current->getNext() != nullptr) {
            current = current->getNext();
            viTri++;
        }
        viTri++; // vì đã có ít nhất 1 phần tử

        sach.setMaSach(ISBN + "_" + StringHelper::toString(viTri));
        current->setNext(new NodeSach(sach));
        return true;
    }

    Sach* timSachTheoMa(string maSach) {
        NodeSach* current = nodeSach;
        while (current != nullptr) {
            if (current->getData().getMaSach() == maSach) {
                return &(current->getData());
            }
            current = current->getNext();
        }
        return nullptr;
    }

    void tangSoLanMuon() {
        soLanMuon++;
    }
};

#endif // DAUSACH_H
