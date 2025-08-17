#ifndef THEDOCGIA_H
#define THEDOCGIA_H

#include <string>
#include "enums.h"
#include "constants.h"
#include "muon_tra.h"

using namespace std;

class TheDocGia {
private:
    int maThe;
    string ho;
    string ten;
    string phai;
    TrangThaiTheDocGia trangThai;
    NodeMuonTra *nodeMuonTra;

public:
    TheDocGia() {
        maThe = 0;
        ho = "";
        ten = "";
        phai = "";
        trangThai = TrangThaiTheDocGia::HOAT_DONG;
        nodeMuonTra = nullptr;
    }

    TheDocGia(string ho, string ten, string phai) {
        this->maThe = 0;
        this->ho = ho;
        this->ten = ten;
        this->phai = phai;
        this->trangThai = TrangThaiTheDocGia::HOAT_DONG;
        nodeMuonTra = nullptr;
    }

    // Getter const
    int getMaThe() const { return maThe; }
    string getHo() const { return ho; }
    string getTen() const { return ten; }
    string getPhai() const { return phai; }
    TrangThaiTheDocGia getTrangThai() const { return trangThai; }
    NodeMuonTra *getNodeMuonTra() const { return nodeMuonTra; }

    // Setter
    void setMaThe(int maThe) { this->maThe = maThe; }
    void setHo(string ho) { this->ho = ho; }
    void setTen(string ten) { this->ten = ten; }
    void setPhai(string phai) { this->phai = phai; }
    void setTrangThai(TrangThaiTheDocGia trangThai) { this->trangThai = trangThai; }
    void setNodeMuonTra(NodeMuonTra *nodeMuonTra) { this->nodeMuonTra = nodeMuonTra; }

    TheDocGia& operator=(const TheDocGia& other) {
        if (this == &other) return *this; // tránh tự gán

        maThe = other.maThe;
        ho = other.ho;
        ten = other.ten;
        phai = other.phai;
        trangThai = other.trangThai;

        // Xoá danh sách cũ nếu có
        clearNodeMuonTra();

        // Deep copy danh sách mượn trả
        nodeMuonTra = copyNodeMuonTra(other.nodeMuonTra);

        return *this;
    }

    // Hàm xoá danh sách mượn trả hiện tại
    void clearNodeMuonTra() {
        NodeMuonTra* temp;
        while (nodeMuonTra) {
            temp = nodeMuonTra;
            nodeMuonTra = nodeMuonTra->getNext();
            delete temp;
        }
        nodeMuonTra = nullptr;
    }

    // Hàm deep copy danh sách mượn trả
    NodeMuonTra* copyNodeMuonTra(NodeMuonTra* head) {
        if (!head) return nullptr;

        NodeMuonTra* newHead = new NodeMuonTra(head->getData()); // copy node đầu tiên
        NodeMuonTra* current = newHead;
        NodeMuonTra* p = head->getNext();

        while (p) {
            current->setNext(new NodeMuonTra(p->getData()));
            current = current->getNext();
            p = p->getNext();
        }

        return newHead;
    }

    // Cho đọc file
    void themMuonTra(MuonTra muonTra) {
        NodeMuonTra *newNode = new NodeMuonTra(muonTra);
        if (nodeMuonTra == nullptr) {
            nodeMuonTra = newNode;
            return;
        }

        // Duyệt đến cuối danh sách
        NodeMuonTra* current = nodeMuonTra;
        while (current->getNext() != nullptr) {
            current = current->getNext();
        }

        current->setNext(newNode);
    }

    void muonSach(MuonTra muonTra) {
        if (trangThai == TrangThaiTheDocGia::BI_KHOA) {
            throw "Thẻ độc giả đã bị khóa";
        }

        int soLuongDangMuon = 0;
        NodeMuonTra* current = nodeMuonTra;

        while (current != nullptr) {
            MuonTra data = current->getData();

            if (data.getNgayTra().isDefault()) {
                // Nếu có sách quá hạn
                if (data.getNgayMuon().daysTo(DateHelper::getCurrentDate()) > MAX_NGAY_MUON_SACH) {
                    throw "Độc giả chưa trả sách trong thời gian cho phép";
                }

                soLuongDangMuon++;
            }

            current = current->getNext();
        }

        if (soLuongDangMuon >= MAX_SL_SACH_MUON) {
            throw "Độc giả đã mượn vượt quá số lượng sách cho phép";
        }

        // Thêm sách mới vào danh sách mượn
        NodeMuonTra *newNode = new NodeMuonTra(muonTra);
        if (nodeMuonTra == nullptr) {
            nodeMuonTra = newNode;
        } else {
            current = nodeMuonTra;
            while (current->getNext() != nullptr) {
                current = current->getNext();
            }
            current->setNext(newNode);
        }
    }

    void traSach(string maSach) {
        // Duyệt đến cuối danh sách
        NodeMuonTra* current = nodeMuonTra;
        if (current == nullptr) {
            throw "Độc giả chưa mượn sách nào";
        }

        while (current != nullptr) {
            if (current->getData().getMaSach() == maSach && current->getData().getNgayTra().isDefault()) {
                current->getData().setNgayTra(DateHelper::getCurrentDate());
                current->getData().setTrangThai(TrangThaiMuonTra::DA_TRA);
                return;
            }

            current = current->getNext();
        }

        throw "Không tìm thấy sách cần trả hoặc sách đã được trả trước đó";
    }

    NodeMuonTra *layDSSachDangMuon() {
        NodeMuonTra *ketQua = nullptr;
        NodeMuonTra *tail = nullptr;
        NodeMuonTra *current = nodeMuonTra;

        while (current != nullptr) {
            if (current->getData().getTrangThai() == TrangThaiMuonTra::CHUA_TRA) {
                NodeMuonTra *newNode = new NodeMuonTra(current->getData());

                if (ketQua == nullptr) {
                    ketQua = newNode;
                    tail = newNode;
                } else {
                    tail->setNext(newNode);
                    tail = newNode;
                }
            }
            current = current->getNext();
        }

        return ketQua;
    }
};

class NodeTheDocGia {
private:
    TheDocGia data;
    NodeTheDocGia *left;
    NodeTheDocGia *right;
public:
    NodeTheDocGia(TheDocGia data) {
        this->data = data;
        this->left = nullptr;
        this->right = nullptr;
    }

    TheDocGia& getData() { return data; }
    const TheDocGia& getData() const { return data; }

    NodeTheDocGia *getLeft() { return left; }
    NodeTheDocGia *getRight() { return right; }

    void setLeft(NodeTheDocGia *left) { this->left = left; }
    void setRight(NodeTheDocGia *right) { this->right = right; }
};

#endif // THEDOCGIA_H
