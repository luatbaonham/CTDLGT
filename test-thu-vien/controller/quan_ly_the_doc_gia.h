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
//tiếng việt
#include <QCollator>
#include <QLocale>
#include <QString>
#include "the_doc_gia.h"
#include "dau_sach.h"
#include "quan_ly_dau_sach.h"
#include "dialog_sua_doc_gia.h"
#include "constants.h"
#include "ma_helper.h"

class QuanLyTheDocGia {
private:
    NodeTheDocGia *root;
    TheDocGia** danhSachTheoTen;  // Mảng tạm theo tên
    int soLuongDocGia;

    int soSanhTenHo(const TheDocGia& a, const TheDocGia& b) {
        static QCollator coll(QLocale(QLocale::Vietnamese, QLocale::Vietnam));
        coll.setCaseSensitivity(Qt::CaseInsensitive);
        coll.setNumericMode(true);               // "A2" < "A10"
        coll.setIgnorePunctuation(true);         // bỏ qua dấu câu nếu có

        const QString tenA = QString::fromStdString(a.getTen()).trimmed().simplified();
        const QString tenB = QString::fromStdString(b.getTen()).trimmed().simplified();

        int cmp = coll.compare(tenA, tenB);
        if (cmp != 0) return cmp;

        const QString hoA  = QString::fromStdString(a.getHo()).trimmed().simplified();
        const QString hoB  = QString::fromStdString(b.getHo()).trimmed().simplified();

        return coll.compare(hoA, hoB);           // tăng dần theo họ khi tên trùng
    }

    // Chèn phần tử mới vào mảng tạm theo đúng vị trí sort
    void chenVaoMangTheoTen(TheDocGia* dg) {
        TheDocGia** newArr = new TheDocGia*[soLuongDocGia + 1];

        int i = 0, j = 0;
        bool daChen = false;

        while (i < soLuongDocGia) {
            if (!daChen && soSanhTenHo(*dg, *danhSachTheoTen[i]) < 0) {
                newArr[j++] = dg;
                daChen = true;
            }
            newArr[j++] = danhSachTheoTen[i++];
        }

        if (!daChen) {
            newArr[j++] = dg;
        }

        delete[] danhSachTheoTen;
        danhSachTheoTen = newArr;
        soLuongDocGia++;
    }

    // Xóa phần tử khỏi mảng tạm theo mã thẻ
    void xoaKhoiMangTheoTen(int maThe) {
        if (!danhSachTheoTen || soLuongDocGia == 0) return;

        int newSize = soLuongDocGia - 1;
        TheDocGia** newArr = new TheDocGia*[newSize];

        int j = 0;
        for (int i = 0; i < soLuongDocGia; i++) {
            if (danhSachTheoTen[i]->getMaThe() != maThe) {
                newArr[j++] = danhSachTheoTen[i];
            }
        }

        delete[] danhSachTheoTen;
        danhSachTheoTen = newArr;
        soLuongDocGia = newSize;
    }

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
    }

    NodeTheDocGia* tim(NodeTheDocGia *node, int maThe) {
        if (node == nullptr || node->getData().getMaThe() == maThe) return node;
        if (maThe < node->getData().getMaThe()) return tim(node->getLeft(), maThe);
        return tim(node->getRight(), maThe);
    }

    void duyetLNR(NodeTheDocGia *root, QTableWidget *table, int &row) {
        if (root == nullptr) return;
        duyetLNR(root->getLeft(), table, row);
        themDocGiaVaoBang(&root->getData(), table, row++);
        duyetLNR(root->getRight(), table, row);
    }

    // Xây lại danh sách theo tên từ cây
    void buildDanhSachTheoTenTuCay(NodeTheDocGia* node) {
        if (!node) return;

        buildDanhSachTheoTenTuCay(node->getLeft());

        TheDocGia* copy = new TheDocGia(node->getData());
        chenVaoMangTheoTen(copy); // sắp xếp vào mảng tạm theo tên

        buildDanhSachTheoTenTuCay(node->getRight());
    }

    void capNhatDocGiaTrongMang(const TheDocGia& docGia) {
        if (!danhSachTheoTen) return;

        // Xóa bản cũ trong mảng
        xoaKhoiMangTheoTen(docGia.getMaThe());

        // Thêm bản mới vào đúng vị trí
        TheDocGia* copy = new TheDocGia(docGia);
        chenVaoMangTheoTen(copy);
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
                // 🔹 cập nhật lại mảng tạm
                this->capNhatDocGiaTrongMang(*docGia);

                // 🔹 refresh bảng theo tên
                this->hienThiDanhSachDocGiaTheoTen(table);
            }
        });

        // Xóa
        QObject::connect(btnXoa, &QPushButton::clicked, [this, maThe, table]() {
            int ret = QMessageBox::warning(table, "Xác nhận xóa",
                                           "Bạn có chắc chắn muốn xóa thẻ độc giả này?",
                                           QMessageBox::Yes | QMessageBox::No);
            if (ret == QMessageBox::Yes) {
                this->xoaTheDocGia(maThe);
                this->hienThiDanhSachDocGiaTheoTen(table);
            }
        });
    }

    void giaiPhong(NodeTheDocGia* node) {
        if (node == nullptr) return;

        giaiPhong(node->getLeft());
        giaiPhong(node->getRight());

        TheDocGia& docGia = node->getData();
        NodeMuonTra* current = docGia.getNodeMuonTra();

        while (current != nullptr) {
            NodeMuonTra* tmp = current;
            current = current->getNext();
            delete tmp;
        }

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
            out << "#\n";
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
            if (cur != nullptr) muonTraStream << ";";
        }

        out << muonTraStream.str() << "\n";

        ghiNode(node->getLeft(), out);
        ghiNode(node->getRight(), out);
    }

    int safeStoi(const string& s, int defaultVal = 0) {
        try {
            if (s.empty()) return defaultVal;
            return stoi(s);
        } catch (...) {
            return defaultVal;
        }
    }

    NodeTheDocGia* docNode(ifstream& in) {
        string line;
        if (!getline(in, line)) return nullptr;
        if (line.empty()) return docNode(in); // bỏ qua dòng trống
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
        dg.setMaThe(safeStoi(maStr));
        dg.setHo(ho);
        dg.setTen(ten);
        dg.setPhai(phai == "0" ? "Nam" : "Nữ");
        dg.setTrangThai((TrangThaiTheDocGia)safeStoi(trangThaiStr));

        stringstream listStream(dsMuonTraStr);
        string muonTraInfo;
        while (getline(listStream, muonTraInfo, ';')) {
            if (muonTraInfo.empty()) continue;
            stringstream sstream(muonTraInfo);
            string maSach, ngayMuonStr, ngayTraStr, trangThaiStr;

            getline(sstream, maSach, ',');
            getline(sstream, ngayMuonStr, ',');
            getline(sstream, ngayTraStr, ',');
            getline(sstream, trangThaiStr);

            int d = 0, m = 0, y = 0;
            sscanf(ngayMuonStr.c_str(), "%d-%d-%d", &d, &m, &y);
            Ngay ngayMuon(d, m, y);

            sscanf(ngayTraStr.c_str(), "%d-%d-%d", &d, &m, &y);
            Ngay ngayTra(d, m, y);

            MuonTra mt;
            mt.setMaSach(maSach);
            mt.setNgayMuon(ngayMuon);
            mt.setNgayTra(ngayTra);
            mt.setTrangThai((TrangThaiMuonTra)safeStoi(trangThaiStr));

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
        danhSachTheoTen = nullptr;
        soLuongDocGia = 0;
        docFile(FILE_THE_DOC_GIA);
        buildDanhSachTheoTenTuCay(root);
        srand(time(0));
    }

    ~QuanLyTheDocGia() {
        ghiFile(FILE_THE_DOC_GIA);
        giaiPhong(root);
        root = nullptr;

        if (danhSachTheoTen) {
            delete[] danhSachTheoTen;
        }
    }

    NodeTheDocGia* getRoot() { return root; }

    void themTheDocGia(TheDocGia data) {
        data.setMaThe(taoMaTheNgauNhien());
        root = them(root, data);

        TheDocGia* dgPtr = timTheDocGia(data.getMaThe());
        if (dgPtr) {
            chenVaoMangTheoTen(dgPtr);
        }
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
        duyetTruoc(root, table, row);
    }

    void hienThiDanhSachDocGiaTheoMa(QTableWidget *table) {
        table->setColumnCount(6);
        table->setHorizontalHeaderLabels({"Mã thẻ", "Họ", "Tên", "Phái", "Trạng thái", "Hành động"});
        table->setRowCount(0);
        int row = 0;
        duyetLNR(root, table, row);
    }

    void hienThiDanhSachDocGiaTheoTen(QTableWidget* table) {
        if (!danhSachTheoTen) return;

        table->setRowCount(0);
        for (int i = 0; i < soLuongDocGia; ++i) {
            themDocGiaVaoBang(danhSachTheoTen[i], table, i);
        }
    }

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

        xoaKhoiMangTheoTen(maThe);
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
