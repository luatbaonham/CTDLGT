#ifndef DIALOG_CHI_TIET_SACH_H
#define DIALOG_CHI_TIET_SACH_H

#include <QDialog>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QPushButton>
#include "dau_sach.h"
#include "enums.h"

class DialogChiTietSach : public QDialog {
    Q_OBJECT

private:
    QTableWidget* tableWidget;

public:
    DialogChiTietSach(DauSach &dauSach, QWidget *parent = nullptr) : QDialog(parent) {
        setWindowTitle("Chi tiết sách");
        resize(500, 300);

        QVBoxLayout *mainLayout = new QVBoxLayout(this);

        tableWidget = new QTableWidget(this);
        tableWidget->setColumnCount(3);
        tableWidget->setHorizontalHeaderLabels({"Mã sách", "Vị trí", "Trạng thái"});
        mainLayout->addWidget(tableWidget);

        QPushButton *btnClose = new QPushButton("Đóng", this);
        connect(btnClose, &QPushButton::clicked, this, &DialogChiTietSach::close);
        mainLayout->addWidget(btnClose);

        hienThiSach(dauSach);
    }

    void hienThiSach(DauSach &dauSach) {
        NodeSach* headSach = dauSach.getNodeSach();
        tableWidget->setRowCount(0);

        while (headSach != nullptr) {
            Sach s = headSach->getData();
            int row = tableWidget->rowCount();
            tableWidget->insertRow(row);

            tableWidget->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(s.getMaSach())));
            tableWidget->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(s.getViTri())));

            QString trangThai;
            switch (s.getTrangThai()) {
                case TrangThaiSach::CHO_MUON_DUOC: trangThai = "Cho mượn được"; break;
                case TrangThaiSach::DA_DUOC_MUON: trangThai = "Đã được mượn"; break;
                case TrangThaiSach::DA_THANH_LY: trangThai = "Đã thanh lý"; break;
                default: trangThai = "Không rõ"; break;
            }

            tableWidget->setItem(row, 2, new QTableWidgetItem(trangThai));
            headSach = headSach->getNext();
        }
    }
};

#endif // DIALOG_CHI_TIET_SACH_H
