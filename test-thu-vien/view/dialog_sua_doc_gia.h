#ifndef DIALOG_SUA_DOC_GIA_H
#define DIALOG_SUA_DOC_GIA_H

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QFormLayout>
#include <QDialogButtonBox>
#include "the_doc_gia.h"
#include "validation_helper.h"

class DialogSuaDocGia : public QDialog {
    Q_OBJECT

private:
    QLineEdit *editHo;
    QLineEdit *editTen;
    QComboBox *comboPhai;
    QComboBox *comboTrangThai;

public:
    DialogSuaDocGia(TheDocGia &docGia, QWidget *parent = nullptr) : QDialog(parent) {
        setWindowTitle("Sửa Thẻ Độc Giả");

        editHo = new QLineEdit(QString::fromStdString(docGia.getHo()));
        editTen = new QLineEdit(QString::fromStdString(docGia.getTen()));

        ValidationHelper::validateString(editHo);
        ValidationHelper::validateString(editTen);

        comboPhai = new QComboBox();
        comboPhai->addItems({"Nam", "Nữ"});
        comboPhai->setCurrentText(QString::fromStdString(docGia.getPhai()));

        comboTrangThai = new QComboBox();
        comboTrangThai->addItems({"Hoạt động", "Khóa"});
        comboTrangThai->setCurrentText(docGia.getTrangThai() == HOAT_DONG ? "Hoạt động" : "Khóa");

        QFormLayout *form = new QFormLayout();
        form->addRow("Họ:", editHo);
        form->addRow("Tên:", editTen);
        form->addRow("Phái:", comboPhai);
        form->addRow("Trạng thái:", comboTrangThai);

        QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
        connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
        connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

        QVBoxLayout *layout = new QVBoxLayout();
        layout->addLayout(form);
        layout->addWidget(buttonBox);
        setLayout(layout);
    }

    void capNhatDocGia(TheDocGia &docGia) {
        docGia.setHo(editHo->text().toStdString());
        docGia.setTen(editTen->text().toStdString());
        docGia.setPhai(comboPhai->currentText().toStdString());
        docGia.setTrangThai(comboTrangThai->currentText() == "Hoạt động" ? HOAT_DONG : BI_KHOA);
    }
};

#endif // DIALOG_SUA_DOC_GIA_H
