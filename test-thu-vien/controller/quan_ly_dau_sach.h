#ifndef QUAN_LY_DAU_SACH_H
#define QUAN_LY_DAU_SACH_H

#include <QTableWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QDir>
#include <fstream>
#include <sstream>
#include "dau_sach.h"
#include "dialog_them_sach.h"
#include "dialog_chi_tiet_sach.h"
#include "constants.h"
#include "string_helper.h"
#include "ma_helper.h"

// DS tuyen tinh
class QuanLyDauSach {
private:
    int soLuong;
    DauSach nodes[MAX_DAU_SACH];

    void ghiFile(string fileName) {
        ofstream out(fileName);
        if (!out.is_open()) return;

        for (int i = 0; i < soLuong; i++) {
            DauSach& ds = nodes[i];
            out << ds.getISBN() << "|"
                << ds.getTenSach() << "|"
                << ds.getSoTrang() << "|"
                << ds.getTacGia() << "|"
                << ds.getNamXuatBan() << "|"
                << ds.getTheLoai() << "|"
                << ds.getSoLanMuon();

            // Ghi danh sách sách
            NodeSach *cur = ds.getNodeSach();
            if (cur != nullptr) out << "|";

            stringstream sachStream;

            while (cur != nullptr) {
                Sach s = cur->getData();
                sachStream << s.getMaSach() << ","
                           << (int)s.getTrangThai() << ","
                           << s.getViTri();

                cur = cur->getNext();
                if (cur != nullptr) sachStream << ";";  // phân cách giữa các sách
            }

            out << sachStream.str() << "\n";
        }

        out.close();
    }

    void docFile(string fileName) {
        ifstream in(fileName);
        if (!in.is_open()) return;

        soLuong = 0;
        string line;
        while (getline(in, line)) {
            if (soLuong >= MAX_DAU_SACH) break;

            stringstream ss(line);
            string isbn, tenSach, soTrangStr, tacGia, namXBStr, theLoai, soLanMuonStr, dsSachStr;

            getline(ss, isbn, '|');
            getline(ss, tenSach, '|');
            getline(ss, soTrangStr, '|');
            getline(ss, tacGia, '|');
            getline(ss, namXBStr, '|');
            getline(ss, theLoai, '|');
            getline(ss, soLanMuonStr, '|');
            getline(ss, dsSachStr);

            // Truy cập trực tiếp nodes[soLuong]
            DauSach& ds = nodes[soLuong];
            ds.setISBN(isbn);
            ds.setTenSach(tenSach);
            ds.setSoTrang(stoi(soTrangStr));
            ds.setTacGia(tacGia);
            ds.setNamXuatBan(stoi(namXBStr));
            ds.setTheLoai(theLoai);
            ds.setSoLanMuon(stoi(soLanMuonStr));

            // Đọc danh sách sách
            stringstream listStream(dsSachStr);
            string sachInfo;
            while (getline(listStream, sachInfo, ';')) {
                stringstream sstream(sachInfo);
                string ma, trangThaiStr, viTri;
                getline(sstream, ma, ',');
                getline(sstream, trangThaiStr, ',');
                getline(sstream, viTri);

                Sach s;
                s.setMaSach(ma);
                s.setTrangThai((TrangThaiSach)stoi(trangThaiStr));
                s.setViTri(viTri);
                ds.themSach(s);
            }

            soLuong++;
        }

        in.close();
    }

    void swap(DauSach *&a, DauSach *&b) {
        DauSach *tmp;
        tmp = a;
        a = b;
        b = tmp;
    }

    void heapifyUp(DauSach *heap[], int index) {
        while (index > 0) {
            int parent = (index - 1) / 2;
            if (heap[index]->getSoLanMuon() < heap[parent]->getSoLanMuon()) {
                swap(heap[index], heap[parent]);
                index = parent;
            } else break;
        }
    }

    void heapifyDown(DauSach *heap[], int size, int index) {
        while (index < size) {
            int smallest = index;
            int left = 2 * index + 1;
            int right = 2 * index + 2;

            if (left < size && heap[left]->getSoLanMuon() < heap[smallest]->getSoLanMuon())
                smallest = left;
            if (right < size && heap[right]->getSoLanMuon() < heap[smallest]->getSoLanMuon())
                smallest = right;

            if (smallest != index) {
                swap(heap[index], heap[smallest]);
                index = smallest;
            } else break;
        }
    }

    void themVaoTop10(DauSach *heap[], int& size, DauSach* ds) {
        if (size < MAX_TOP_DAU_SACH) {
            heap[size] = ds;
            heapifyUp(heap, size);
            size++;
        } else if (ds->getSoLanMuon() > heap[0]->getSoLanMuon()) {
            heap[0] = ds;
            heapifyDown(heap, size, 0);
        }
    }

public:
    QuanLyDauSach() {
        this->soLuong = 0;
        docFile(FILE_DAU_SACH);
    }

    ~QuanLyDauSach() {
        ghiFile(FILE_DAU_SACH);

        for (DauSach& ds : nodes) {
            NodeSach* current = ds.getNodeSach();
            while (current != nullptr) {
                NodeSach* tmp = current;
                current = current->getNext();
                delete tmp;
            }
            ds.setNodeSach(nullptr); // tránh dangling pointer
        }
    }

    int getSoLuong() { return soLuong; }
    DauSach *getNodes() { return nodes; }

    void themDauSach(DauSach ds) {
        if (soLuong >= MAX_DAU_SACH) {
            throw "Không thể thêm, danh sách đầy.";
        }

        int viTriChen = 0;
        while (viTriChen < soLuong && nodes[viTriChen].getTenSach() < ds.getTenSach()) {
            viTriChen++;
        }

        for (int i = soLuong; i > viTriChen; i--) {
            nodes[i] = nodes[i - 1];  // Dời phần tử sang phải
        }

        nodes[viTriChen] = ds;
        soLuong++;
    }


    DauSach* timTheoISBN(string ISBN) {
        for (int i = 0; i < soLuong; i++) {
            if (nodes[i].getISBN() == ISBN) return &nodes[i];
        }
        return nullptr;
    }

    Sach *timSachTheoMaSach(string maSach) {
        string isbn = MaHelper::layISBNTheoMaSach(maSach);
        if (isbn.empty()) {
            return nullptr;
        }

        DauSach *dauSach = timTheoISBN(isbn);
        if (dauSach == nullptr) {
            return nullptr;
        }

        return dauSach->timSachTheoMa(maSach);
    }

    void hienThiDanhSachDauSach(string tenSach, string theLoai, QTableWidget* table) {
        table->setColumnCount(7);
        table->setHorizontalHeaderLabels({"ISBN", "Tên sách", "Số trang", "Tác giả", "Năm XB", "Thể loại", "Hành động"});
        table->setRowCount(0);

        for (int i = 0; i < soLuong; i++) {
            bool matchTheLoai = theLoai == "Tất cả" || theLoai.empty() || nodes[i].getTheLoai() == theLoai;
            bool matchTen = tenSach.empty() || StringHelper::toUpper(nodes[i].getTenSach()).find(StringHelper::toUpper(tenSach)) != string::npos;

            if (matchTheLoai && matchTen) {
                int row = table->rowCount();
                table->insertRow(row);

                table->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(nodes[i].getISBN())));
                table->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(nodes[i].getTenSach())));
                table->setItem(row, 2, new QTableWidgetItem(QString::number(nodes[i].getSoTrang())));
                table->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(nodes[i].getTacGia())));
                table->setItem(row, 4, new QTableWidgetItem(QString::number(nodes[i].getNamXuatBan())));
                table->setItem(row, 5, new QTableWidgetItem(QString::fromStdString(nodes[i].getTheLoai())));

                QWidget *actionWidget = new QWidget();
                QPushButton *btnThemSach = new QPushButton("Thêm sách");
                QString isbn = QString::fromStdString(nodes[i].getISBN());
                btnThemSach->setProperty("isbn", isbn);

                QPushButton *btnChiTiet = new QPushButton("Chi tiết");
                btnChiTiet->setProperty("isbn", isbn);

                QHBoxLayout *layout = new QHBoxLayout(actionWidget);
                layout->addWidget(btnThemSach);
                layout->addWidget(btnChiTiet);
                layout->setContentsMargins(0, 0, 0, 0);
                actionWidget->setLayout(layout);

                table->setCellWidget(row, 6, actionWidget);

                QObject::connect(btnThemSach, &QPushButton::clicked, [this, isbn, table]() {
                    DauSach* ds = timTheoISBN(isbn.toStdString());
                    if (ds == nullptr) {
                        QMessageBox::warning(table, "Lỗi", "Không tìm thấy đầu sách!");
                        return;
                    }

                    DialogThemSach dialog(*ds, table);
                    if (dialog.exec() == QDialog::Accepted) {
                        dialog.capNhatDanhSachSach();
                    }
                });

                QObject::connect(btnChiTiet, &QPushButton::clicked, [this, isbn, table]() {
                    DauSach* ds = timTheoISBN(isbn.toStdString());
                    if (ds == nullptr) {
                        QMessageBox::warning(table, "Lỗi", "Không tìm thấy đầu sách!");
                        return;
                    }

                    DialogChiTietSach dialog(*ds, table);
                    dialog.exec();
                });
            }
        }
    }

    void hienThiTop10Sach(QTableWidget* table) {
        table->setRowCount(0);

        DauSach *heapTop10[MAX_TOP_DAU_SACH];
        int heapSize = 0;

        for (int i = 0; i < soLuong; i++) {
            themVaoTop10(heapTop10, heapSize, &nodes[i]);
        }

        for (int i = heapSize / 2 - 1; i >= 0; i--)
            heapifyDown(heapTop10, heapSize, i);
        for (int i = heapSize - 1; i >= 1; i--) {
            swap(heapTop10[0], heapTop10[i]);
            heapifyDown(heapTop10, i, 0);
        }

        for (int i = 0; i < soLuong; i++) {
            int row = table->rowCount();
            table->insertRow(row);

            table->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(nodes[i].getISBN())));
            table->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(nodes[i].getTenSach())));
            table->setItem(row, 2, new QTableWidgetItem(QString::number(nodes[i].getSoLanMuon())));
        }
    }
};

#endif // QUAN_LY_DAU_SACH_H
