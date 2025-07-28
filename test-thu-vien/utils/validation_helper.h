#ifndef VALIDATION_HELPER_H
#define VALIDATION_HELPER_H

#include <QLineEdit>
#include <QValidator>
#include <QMessageBox>
#include <climits>

class ValidationHelper {
public:
    static void validateString(QLineEdit* lineEdit) {
        QLineEdit::connect(lineEdit, &QLineEdit::textChanged, lineEdit, [=](const QString &text) {
            QString newText = text;

            // Xóa khoảng trắng đầu
            if (newText.startsWith(" ")) {
                newText.remove(0, 1);
            }

            // Thay thế 2 khoảng trắng liên tiếp thành 1
            while (newText.contains("  ")) {
                newText.replace("  ", " ");
            }

            // Nếu có thay đổi, cập nhật lại lineEdit (tránh lặp vô hạn)
            if (newText != text) {
                int cursorPos = lineEdit->cursorPosition();
                lineEdit->setText(newText);
                lineEdit->setCursorPosition(qMin(cursorPos - 1, newText.length()));
            }
        });

        QLineEdit::connect(lineEdit, &QLineEdit::editingFinished, lineEdit, [=]() {
            QString text = lineEdit->text();

            // Xóa khoảng trắng cuối
            while (text.endsWith(" ")) {
                text.chop(1); // Xóa 1 ký tự cuối
            }

            lineEdit->setText(text);
        });
    }

    static void validateInt(QLineEdit* lineEdit, int min = 0, int max = INT_MAX / 10) {
        lineEdit->setValidator(new QIntValidator(min, max, lineEdit));

        // Sử dụng textChanged thay vì editingFinished
        QObject::connect(lineEdit, &QLineEdit::textChanged, lineEdit, [=]() {
            QString text = lineEdit->text();
            if (text.isEmpty()) return; // Cho phép trống tạm thời

            bool ok;
            int value = text.toInt(&ok);
            if (!ok || value < min || value > max) {
                // Lưu lại vị trí con trỏ
                int cursorPos = lineEdit->cursorPosition();

                // Xóa ký tự cuối cùng vừa nhập
                QString newText = text.left(text.length() - 1);
                lineEdit->setText(newText);

                // Khôi phục vị trí con trỏ
                lineEdit->setCursorPosition(qMin(cursorPos - 1, newText.length()));
            }
        });
    }
};

#endif // VALIDATION_HELPER_H
