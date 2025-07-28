#ifndef QUAN_LY_THE_DOC_GIA_H
#define QUAN_LY_THE_DOC_GIA_H

#include <QTableWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QWidget>
#include <QMessageBox>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include "the_doc_gia.h"
#include "dau_sach.h"
#include "quan_ly_dau_sach.h"
#include "dialog_sua_doc_gia.h"
#include "constants.h"
#include "ma_helper.h"

class QuanLyTheDocGia {
private:
    NodeTheDocGia *root;

    int taoMaTheNgauNhien() {
        int ma;
        do {
            ma = 10000 + rand() % 90000;
        } while (tim(root, ma) != nullptr);
        return ma;
    }
    void duyetTruoc(NodeTheDocGia* node, QTableWidget* table, int& row) {
        if (node == nullptr) return;
        themDocGiaVaoBang(&node->getData(), table, row++);
        duyetTruoc(node->getLeft(), table, row);
        duyetTruoc(node->getRight(), table, row);
    }


    NodeTheDocGia* them(NodeTheDocGia *node, TheDocGia data) {
        if (node == nullptr) return new NodeTheDocGia(data);
        if (data.getMaThe() < node->getData().getMaThe()) node->setLeft(them(node->getLeft(), data));
        else if (data.getMaThe() > node->getData().getMaThe()) node->setRight(them(node->getRight(), data));
        return node;
    }//cập nhật cây


    NodeTheDocGia* tim(NodeTheDocGia *node, int maThe) {
        if (node == nullptr || node->getData().getMaThe() == maThe) return node;
        if (maThe < node->getData().getMaThe()) return tim(node->getLeft(), maThe);
        return tim(node->getRight(), maThe);
    }//tìm node vừa thêm

    void duyetLNR(NodeTheDocGia *root, QTableWidget *table, int &row) {
        if (root == nullptr) return;
        duyetLNR(root->getLeft(), table, row);
        themDocGiaVaoBang(&root->getData(), table, row++);
        duyetLNR(root->getRight(), table, row);
    }

    void themDocGiaVaoBang(TheDocGia* dg, QTableWidget* table, int row) {
        int maThe = dg->getMaThe();

        table->insertRow(row);
        table->setItem(row, 0, new QTableWidgetItem(QString::number(maThe)));
        table->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(dg->getHo())));
        table->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(dg->getTen())));
        table->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(dg->getPhai())));
        table->setItem(row, 4, new QTableWidgetItem(dg->getTrangThai() == TrangThaiTheDocGia::HOAT_DONG ? "Hoạt động" : "Khóa"));

        QWidget *actionWidget = new QWidget();
        QPushButton *btnSua = new QPushButton("Sửa");
        QPushButton *btnXoa = new QPushButton("Xóa");
        btnSua->setProperty("maThe", maThe);
        btnXoa->setProperty("maThe", maThe);

        QHBoxLayout *layout = new QHBoxLayout(actionWidget);
        layout->addWidget(btnSua);
        layout->addWidget(btnXoa);
        layout->setContentsMargins(0, 0, 0, 0);
        actionWidget->setLayout(layout);
        table->setCellWidget(row, 5, actionWidget);

        // Sửa
        QObject::connect(btnSua, &QPushButton::clicked, [this, maThe, table]() {
            TheDocGia* docGia = this->timTheDocGia(maThe);
            if (!docGia) return;

            DialogSuaDocGia dialog(*docGia, table);
            if (dialog.exec() == QDialog::Accepted) {
                dialog.capNhatDocGia(*docGia);
                this->hienThiDanhSachDocGia(table); // hoặc gọi lại theo loại sắp xếp hiện tại
            }
        });

        // Xóa
        QObject::connect(btnXoa, &QPushButton::clicked, [this, maThe, table]() {
            int ret = QMessageBox::warning(table, "Xác nhận xóa",
                                           "Bạn có chắc chắn muốn xóa thẻ độc giả này?",
                                           QMessageBox::Yes | QMessageBox::No);
            if (ret == QMessageBox::Yes) {
                this->xoaTheDocGia(maThe);
                this->hienThiDanhSachDocGia(table); // hoặc tương ứng
            }
        });
    }

    void giaiPhong(NodeTheDocGia* node) {
        if (node == nullptr) return;

        giaiPhong(node->getLeft());
        giaiPhong(node->getRight());

        // Tránh gọi getData() nhiều lần
        TheDocGia& docGia = node->getData();
        NodeMuonTra* current = docGia.getNodeMuonTra();

        while (current != nullptr) {
            NodeMuonTra* tmp = current;
            current = current->getNext();
            delete tmp;
        }

        // Đảm bảo không để con trỏ treo trong đối tượng gốc
        docGia.setNodeMuonTra(nullptr);

        delete node;
    }

    void ghiFile(string fileName) {
        ofstream out(fileName);
        if (!out.is_open()) return;

        ghiNode(root, out);
        out.close();
    }

    void docFile(string fileName) {
        ifstream in(fileName);
        if (!in.is_open()) return;

        root = docNode(in);

        in.close();
    }

    void ghiNode(NodeTheDocGia* node, ofstream& out) {
        if (node == nullptr) {
            out << "#\n";  // Đánh dấu node rỗng
            return;
        }

        TheDocGia& dg = node->getData();
        out << dg.getMaThe() << "|"
            << dg.getHo() << "|"
            << dg.getTen() << "|"
            << (dg.getPhai() == "Nam" ? "0" : "1") << "|"
            << (int)dg.getTrangThai();

        NodeMuonTra *cur = dg.getNodeMuonTra();
        if (cur != nullptr) out << "|";

        stringstream muonTraStream;

        while (cur != nullptr) {
            MuonTra mt = cur->getData();
            Ngay muon = mt.getNgayMuon();
            Ngay tra = mt.getNgayTra();

            muonTraStream << mt.getMaSach() << ","
                          << muon.getNgay() << "-" << muon.getThang() << "-" << muon.getNam() << ","
                          << tra.getNgay() << "-" << tra.getThang() << "-" << tra.getNam() << ","
                          << (int)mt.getTrangThai();

            cur = cur->getNext();
            if (cur != nullptr) muonTraStream << ";";  // phân cách giữa các sách
        }

        out << muonTraStream.str() << "\n";

        ghiNode(node->getLeft(), out);
        ghiNode(node->getRight(), out);
    }

    NodeTheDocGia* docNode(ifstream& in) {
        string line;
        if (!getline(in, line)) return nullptr;

        if (line == "#") return nullptr;

        stringstream ss(line);
        string maStr, ho, ten, phai, trangThaiStr, dsMuonTraStr;

        getline(ss, maStr, '|');
        getline(ss, ho, '|');
        getline(ss, ten, '|');
        getline(ss, phai, '|');
        getline(ss, trangThaiStr, '|');
        getline(ss, dsMuonTraStr);

        TheDocGia dg;
        dg.setMaThe(stoi(maStr));
        dg.setHo(ho);
        dg.setTen(ten);
        dg.setPhai(phai == "0" ? "Nam" : "Nữ");
        dg.setTrangThai((TrangThaiTheDocGia)stoi(trangThaiStr));

        stringstream listStream(dsMuonTraStr);
        string muonTraInfo;
        NodeMuonTra *nodeMuonTra = nullptr;
        while (getline(listStream, muonTraInfo, ';')) {
            stringstream sstream(muonTraInfo);
            string maSach, ngayMuonStr, ngayTraStr, trangThaiStr;

            getline(sstream, maSach, ',');
            getline(sstream, ngayMuonStr, ',');
            getline(sstream, ngayTraStr, ',');
            getline(sstream, trangThaiStr);

            int d, m, y;
            sscanf(ngayMuonStr.c_str(), "%d-%d-%d", &d, &m, &y);
            Ngay ngayMuon(d, m, y);

            sscanf(ngayTraStr.c_str(), "%d-%d-%d", &d, &m, &y);
            Ngay ngayTra(d, m, y);

            MuonTra mt;
            mt.setMaSach(maSach);
            mt.setNgayMuon(ngayMuon);
            mt.setNgayTra(ngayTra);
            mt.setTrangThai((TrangThaiMuonTra)stoi(trangThaiStr));

            dg.themMuonTra(mt);
        }

        NodeTheDocGia *node = new NodeTheDocGia(dg);
        node->setLeft(docNode(in));
        node->setRight(docNode(in));
        return node;
    }

public:
    QuanLyTheDocGia() {
        root = nullptr;
        docFile(FILE_THE_DOC_GIA);
        srand(time(0));
    }

    ~QuanLyTheDocGia() {
        ghiFile(FILE_THE_DOC_GIA);
        giaiPhong(root);
        root = nullptr;
    }

    NodeTheDocGia* getRoot() { return root; }

    void themTheDocGia(TheDocGia data) {
        data.setMaThe(taoMaTheNgauNhien());
        root = them(root, data);
    }

    TheDocGia* timTheDocGia(int maThe) {
        NodeTheDocGia *node = tim(root, maThe);
        if (node == nullptr) return nullptr;
        return &node->getData();
    }

    void hienThiDanhSachDocGia(QTableWidget *table) {
        table->setColumnCount(6);
        table->setHorizontalHeaderLabels({"Mã thẻ", "Họ", "Tên", "Phái", "Trạng thái", "Hành động"});
        table->setRowCount(0);
        int row = 0;
        duyetTruoc(root, table, row);  // <<== sửa ở đây
    }

    void hienThiDanhSachDocGiaTheoMa(QTableWidget *table) {
        table->setColumnCount(6);
        table->setHorizontalHeaderLabels({"Mã thẻ", "Họ", "Tên", "Phái", "Trạng thái", "Hành động"});
        table->setRowCount(0);
        int row = 0;
        duyetLNR(root, table, row);  // duyệt theo thứ tự mã thẻ tăng dần
    }

    void layDanhSachDocGia(NodeTheDocGia* node, TheDocGia** arr, int& index) {
        if (node == nullptr) return;
        layDanhSachDocGia(node->getLeft(), arr, index);
        arr[index++] = &node->getData();
        layDanhSachDocGia(node->getRight(), arr, index);
    }
    int demSoDocGia(NodeTheDocGia* node) {
        if (node == nullptr) return 0;
        return 1 + demSoDocGia(node->getLeft()) + demSoDocGia(node->getRight());
    }
    void SapXepTangTheoTenVaHo(TheDocGia** temp, int soLuong) {
        TheDocGia* tmp;
        char ten1[100], ten2[100];
        char ho1[100], ho2[100];

        for (int i = 0; i < soLuong - 1; i++) {
            for (int j = i + 1; j < soLuong; j++) {
                // Copy tên
                strcpy(ten1, temp[i]->getTen().c_str());
                strcpy(ten2, temp[j]->getTen().c_str());
                // So sánh tên trước
                if (_stricmp(ten1, ten2) > 0) {
                    tmp = temp[i];
                    temp[i] = temp[j];
                    temp[j] = tmp;
                } else if (_stricmp(ten1, ten2) == 0) {
                    // Nếu tên giống nhau, so sánh họ
                    strcpy(ho1, temp[i]->getHo().c_str());
                    strcpy(ho2, temp[j]->getHo().c_str());
                    if (_stricmp(ho1, ho2) > 0) {
                        tmp = temp[i];
                        temp[i] = temp[j];
                        temp[j] = tmp;
                    }
                }
            }
        }
    }


    void hienThiDanhSachDocGiaTheoTen(QTableWidget* table) {
        int soLuong = demSoDocGia(root);
        if (soLuong == 0) return;

        TheDocGia** danhSach = new TheDocGia*[soLuong];
        int index = 0;
        layDanhSachDocGia(root, danhSach, index);

        SapXepTangTheoTenVaHo(danhSach, soLuong);

        table->setColumnCount(6);
        table->setHorizontalHeaderLabels({"Mã thẻ", "Họ", "Tên", "Phái", "Trạng thái", "Hành động"});
        table->setRowCount(0);

        for (int i = 0; i < soLuong; ++i) {
            themDocGiaVaoBang(danhSach[i], table, i);
        }

        delete[] danhSach;
    }




    // void layTatCaDocGia(NodeTheDocGia* node, std::vector<TheDocGia*>& ds) {
    //     if (node == nullptr) return;
    //     layTatCaDocGia(node->getLeft(), ds);
    //     ds.push_back(&node->getData()); // node->data là TheDocGia
    //     layTatCaDocGia(node->getRight(), ds);
    // }

    // void hienThiDanhSachDocGiaTheoTen(QTableWidget *table) {
    //     std::vector<TheDocGia*> danhSach;
    //     layTatCaDocGia(root, danhSach);

    //     std::sort(danhSach.begin(), danhSach.end(), [](TheDocGia* a, TheDocGia* b) {
    //         if (a->getTen() != b->getTen())
    //             return a->getTen() < b->getTen();
    //         return a->getHo() < b->getHo();
    //     });

    //     table->setColumnCount(6);
    //     table->setHorizontalHeaderLabels({"Mã thẻ", "Họ", "Tên", "Phái", "Trạng thái", "Hành động"});
    //     table->setRowCount(0);

    //     for (int row = 0; row < danhSach.size(); ++row) {
    //         themDocGiaVaoBang(danhSach[row], table, row);
    //     }
    // }


    void xoaTheDocGia(int maThe) {
        // Tìm node cha và node cần xóa
        NodeTheDocGia *parent = nullptr;
        NodeTheDocGia *current = root;

        while (current != nullptr && current->getData().getMaThe() != maThe) {
            parent = current;
            if (maThe < current->getData().getMaThe())
                current = current->getLeft();
            else
                current = current->getRight();
        }

        if (current == nullptr) return; // Không tìm thấy

        // Trường hợp 1: Node không có con
        if (current->getLeft() == nullptr && current->getRight() == nullptr) {
            if (parent == nullptr) {
                root = nullptr;
            } else if (parent->getLeft() == current) {
                parent->setLeft(nullptr);
            } else {
                parent->setRight(nullptr);
            }
            delete current;
        }
        // Trường hợp 2: Node có một con
        else if (current->getLeft() == nullptr || current->getRight() == nullptr) {
            NodeTheDocGia* child = (current->getLeft() != nullptr) ? current->getLeft() : current->getRight();

            if (parent == nullptr) {
                root = child;
            } else if (parent->getLeft() == current) {
                parent->setLeft(child);
            } else {
                parent->setRight(child);
            }
            delete current;
        }
        // Trường hợp 3: Node có hai con
        else {
            // Tìm node kế thừa (node nhỏ nhất bên phải)
            NodeTheDocGia* successorParent = current;
            NodeTheDocGia* successor = current->getRight();

            while (successor->getLeft() != nullptr) {
                successorParent = successor;
                successor = successor->getLeft();
            }

            // Sao chép dữ liệu kế thừa vào node hiện tại
            current->getData().setMaThe(successor->getData().getMaThe());
            current->getData().setHo(successor->getData().getHo());
            current->getData().setTen(successor->getData().getTen());
            current->getData().setPhai(successor->getData().getPhai());
            current->getData().setTrangThai(successor->getData().getTrangThai());

            // Xóa node kế thừa
            if (successorParent->getLeft() == successor)
                successorParent->setLeft(successor->getRight());
            else
                successorParent->setRight(successor->getRight());

            delete successor;
        }
    }

    void hienThiDanhSachSachDangMuon(TheDocGia &theDocGia, QuanLyDauSach &qlDauSach, QTableWidget *tableWidget) {
        NodeMuonTra *node = theDocGia.layDSSachDangMuon();

        tableWidget->setRowCount(0);

        while (node != nullptr) {
            MuonTra mt = node->getData();
            string isbn = MaHelper::layISBNTheoMaSach(mt.getMaSach());
            DauSach *ds = qlDauSach.timTheoISBN(isbn);
            if (ds == nullptr) {
                continue;
            }

            int row = tableWidget->rowCount();
            tableWidget->insertRow(row);

            tableWidget->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(mt.getMaSach())));
            tableWidget->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(ds->getTenSach())));
            tableWidget->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(StringHelper::toString(mt.getNgayMuon()))));
            node = node->getNext();
        }
    }
};

#endif // QUAN_LY_THE_DOC_GIA_H
