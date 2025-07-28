#ifndef DIALOG_THEM_SACH_H
#define DIALOG_THEM_SACH_H

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QMessageBox>
#include "sach.h"
#include "dau_sach.h"
#include "validation_helper.h"

class DialogThemSach : public QDialog {
    Q_OBJECT

private:
    QLineEdit *editViTri;
    DauSach& dauSach;

public:
    DialogThemSach(DauSach& ds, QWidget* parent = nullptr) : QDialog(parent), dauSach(ds) {
        setWindowTitle("Thêm sách");

        editViTri = new QLineEdit(QString::fromStdString(""));
        ValidationHelper::validateString(editViTri);

        QFormLayout *form = new QFormLayout();
        form->addRow("Vị trí:", editViTri);

        QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
        connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
        connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

        QVBoxLayout *layout = new QVBoxLayout();
        layout->addLayout(form);
        layout->addWidget(buttonBox);
        setLayout(layout);
    }

    void capNhatDanhSachSach() {
        Sach sach = Sach(editViTri->text().toStdString());

        if (dauSach.themSach(sach)) {
            QMessageBox::information(this, "Thành công", "Đã thêm sách.");
        } else {
            QMessageBox::warning(this, "Lỗi", "Không thể thêm");
        }
    }
};

#endif // DIALOG_THEM_SACH_H
