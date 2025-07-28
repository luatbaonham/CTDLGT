#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QRadioButton>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include "quan_ly_the_doc_gia.h"
#include "quan_ly_dau_sach.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QuanLyTheDocGia qlTheDocGia;
    QuanLyDauSach qlDauSach;

    // Tab widget
    QTabWidget *tabWidget;

    // Tab 1: Quản lý thẻ độc giả
    QGroupBox *groupBox_nhapDocGia;
    QLineEdit *lineEdit_ho;
    QLineEdit *lineEdit_ten;
    QComboBox *comboBox_phai;
    QComboBox *comboBox_trangThaiThe;
    QPushButton *btn_themDocGia;
    QGroupBox *groupBox_danhSachDocGia;
    QRadioButton *radioButton_theoTen;
    QRadioButton *radioButton_theoMa;
    QPushButton *btn_inDanhSachDocGia;
    QTableWidget *tableWidget_docGia;

    // Tab 2: Quản lý đầu sách
    QGroupBox *groupBox_nhapDauSach;
    QLineEdit *lineEdit_isbn;
    QLineEdit *lineEdit_tenSach;
    QLineEdit *lineEdit_tacGia;
    QLineEdit *lineEdit_soTrang;
    QLineEdit *lineEdit_namXuatBan;
    QComboBox *comboBox_theLoai;
    QLineEdit *lineEdit_maSach;
    QPushButton *btn_themDauSach;
    QGroupBox *groupBox_danhSachDauSach;
    QLineEdit *lineEdit_timTenSach;
    QComboBox *comboBox_locTheLoai;
    QTableWidget *tableWidget_dauSach;
    QPushButton *btn_locDauSach;

    // Tab 3: Mượn/Trả sách
    QGroupBox *groupBox_muonSach;
    QLineEdit *lineEdit_maTheMuon;
    QLineEdit *lineEdit_maSachMuon;
    QPushButton *btn_muonSach;
    QGroupBox *groupBox_traSach;
    QLineEdit *lineEdit_maTheTra;
    QLineEdit *lineEdit_maSachTra;
    QPushButton *btn_traSach;
    QGroupBox *groupBox_sachDangMuon;
    QLineEdit *lineEdit_maTheXem;
    QPushButton *btn_xemSachDangMuon;
    QTableWidget *tableWidget_sachDangMuon;

    // Tab 4: Thống kê
    QGroupBox *groupBox_quaHan;
    QTableWidget *tableWidget_quaHan;
    QPushButton *btn_inQuaHan;
    QGroupBox *groupBox_topSach;
    QTableWidget *tableWidget_topSach;
    QPushButton *btn_inTopSach;

private slots:
    // Slots cho Tab 1
    void on_btn_themDocGia_clicked();
    void on_btn_inDanhSachDocGia_clicked();

    // Slots cho Tab 2
    void on_btn_themDauSach_clicked();
    void on_btn_timSach_clicked();
    void on_btn_locDauSach_clicked();

    // Slots cho Tab 3
    void on_btn_muonSach_clicked();
    void on_btn_traSach_clicked();
    void on_btn_xemSachDangMuon_clicked();

    // Slots cho Tab 4
    void on_btn_inQuaHan_clicked();
    void on_btn_inTopSach_clicked();
};

#endif // MAINWINDOW_H
