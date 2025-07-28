#include <QMessageBox>
#include "mainwindow.h"
#include "the_doc_gia.h"
#include "dau_sach.h"
#include "validation_helper.h"
#include "ma_helper.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {

    QDir().mkpath("data");

    // Thiết lập cửa sổ chính
    setWindowTitle("Library Management System");
    resize(800, 600);

    // Tạo QTabWidget
    tabWidget = new QTabWidget(this);
    setCentralWidget(tabWidget);

    // --- Tab 1: Quản lý thẻ độc giả ---
    QWidget *tab_docGia = new QWidget();
    QVBoxLayout *layout_docGia = new QVBoxLayout(tab_docGia);

    // GroupBox: Nhập thông tin độc giả
    groupBox_nhapDocGia = new QGroupBox("Nhập thông tin độc giả", tab_docGia);
    QFormLayout *formLayout_docGia = new QFormLayout();
    lineEdit_ho = new QLineEdit();
    lineEdit_ten = new QLineEdit();
    comboBox_phai = new QComboBox();
    comboBox_phai->addItems({"Nam", "Nữ"});
    // comboBox_trangThaiThe = new QComboBox();
    // comboBox_trangThaiThe->addItems({"0 - Khóa", "1 - Hoạt động"});
    formLayout_docGia->addRow("Họ:", lineEdit_ho);
    formLayout_docGia->addRow("Tên:", lineEdit_ten);
    formLayout_docGia->addRow("Phái:", comboBox_phai);
    // formLayout_docGia->addRow("Trạng thái:", comboBox_trangThaiThe);
    // formLayout_docGia->addRow("Mã thẻ:", lineEdit_maThe);
    groupBox_nhapDocGia->setLayout(formLayout_docGia);

    QHBoxLayout *layout_buttons_docGia = new QHBoxLayout();
    btn_themDocGia = new QPushButton("Thêm");
    layout_buttons_docGia->addWidget(btn_themDocGia);
    formLayout_docGia->addRow(layout_buttons_docGia);

    // GroupBox: Danh sách độc giả
    groupBox_danhSachDocGia = new QGroupBox("Danh sách độc giả", tab_docGia);
    QVBoxLayout *layout_danhSachDocGia = new QVBoxLayout();
    QHBoxLayout *layout_sort_docGia = new QHBoxLayout();
    radioButton_theoTen = new QRadioButton("Sắp xếp theo tên");
    radioButton_theoTen->setAutoExclusive(false);
    radioButton_theoMa = new QRadioButton("Sắp xếp theo mã");
    radioButton_theoMa->setAutoExclusive(false);
    btn_inDanhSachDocGia = new QPushButton("In danh sách");
    layout_sort_docGia->addWidget(radioButton_theoTen);
    layout_sort_docGia->addWidget(radioButton_theoMa);
    layout_sort_docGia->addWidget(btn_inDanhSachDocGia);
    tableWidget_docGia = new QTableWidget();
    tableWidget_docGia->setColumnCount(5);
    tableWidget_docGia->setHorizontalHeaderLabels({"Mã thẻ", "Họ", "Tên", "Phái", "Trạng thái"});
    layout_danhSachDocGia->addLayout(layout_sort_docGia);
    layout_danhSachDocGia->addWidget(tableWidget_docGia);
    groupBox_danhSachDocGia->setLayout(layout_danhSachDocGia);

    layout_docGia->addWidget(groupBox_nhapDocGia);
    layout_docGia->addWidget(groupBox_danhSachDocGia);
    tabWidget->addTab(tab_docGia, "Quản lý thẻ độc giả");

    // --- Tab 2: Quản lý đầu sách ---
    QWidget *tab_dauSach = new QWidget();
    QVBoxLayout *layout_dauSach = new QVBoxLayout(tab_dauSach);

    // GroupBox: Nhập thông tin đầu sách
    groupBox_nhapDauSach = new QGroupBox("Nhập thông tin đầu sách", tab_dauSach);
    QFormLayout *formLayout_dauSach = new QFormLayout();
    lineEdit_isbn = new QLineEdit();
    lineEdit_tenSach = new QLineEdit();
    lineEdit_tacGia = new QLineEdit();
    lineEdit_soTrang = new QLineEdit();
    lineEdit_namXuatBan = new QLineEdit();
    comboBox_theLoai = new QComboBox();
    comboBox_theLoai->addItems({"Văn học", "Khoa học", "Lịch sử", "Khác"});
    formLayout_dauSach->addRow("ISBN:", lineEdit_isbn);
    formLayout_dauSach->addRow("Tên sách:", lineEdit_tenSach);
    formLayout_dauSach->addRow("Tác giả:", lineEdit_tacGia);
    formLayout_dauSach->addRow("Số trang:", lineEdit_soTrang);
    formLayout_dauSach->addRow("Năm xuất bản:", lineEdit_namXuatBan);
    formLayout_dauSach->addRow("Thể loại:", comboBox_theLoai);
    QHBoxLayout *layout_buttons_dauSach = new QHBoxLayout();
    btn_themDauSach = new QPushButton("Thêm đầu sách");
    layout_buttons_dauSach->addWidget(btn_themDauSach);
    formLayout_dauSach->addRow(layout_buttons_dauSach);
    groupBox_nhapDauSach->setLayout(formLayout_dauSach);

    // GroupBox: Danh sách đầu sách
    groupBox_danhSachDauSach = new QGroupBox("Danh sách đầu sách", tab_dauSach);
    QVBoxLayout *layout_danhSachDauSach = new QVBoxLayout();
    QHBoxLayout *layout_locTheLoai = new QHBoxLayout();
    comboBox_locTheLoai = new QComboBox();
    comboBox_locTheLoai->addItems({"Tất cả", "Văn học", "Khoa học", "Lịch sử", "Khác"});
    layout_locTheLoai->addWidget(new QLabel("Lọc thể loại:"));
    layout_locTheLoai->addWidget(comboBox_locTheLoai);
    lineEdit_timTenSach = new QLineEdit();
    lineEdit_timTenSach->setPlaceholderText(QString::fromStdString("Nhập tên sách cần tìm"));
    layout_locTheLoai->addWidget(lineEdit_timTenSach);
    btn_locDauSach = new QPushButton("In danh sách");
    layout_locTheLoai->addWidget(btn_locDauSach);
    tableWidget_dauSach = new QTableWidget();
    layout_danhSachDauSach->addLayout(layout_locTheLoai);
    layout_danhSachDauSach->addWidget(tableWidget_dauSach);
    groupBox_danhSachDauSach->setLayout(layout_danhSachDauSach);

    layout_dauSach->addWidget(groupBox_nhapDauSach);
    // layout_dauSach->addWidget(groupBox_timSach);
    layout_dauSach->addWidget(groupBox_danhSachDauSach);
    tabWidget->addTab(tab_dauSach, "Quản lý đầu sách");

    // --- Tab 3: Mượn/Trả sách ---
    QWidget *tab_muonTra = new QWidget();
    QVBoxLayout *layout_muonTra = new QVBoxLayout(tab_muonTra);

    // GroupBox: Mượn sách
    groupBox_muonSach = new QGroupBox("Mượn sách", tab_muonTra);
    QFormLayout *formLayout_muonSach = new QFormLayout();
    lineEdit_maTheMuon = new QLineEdit();
    lineEdit_maSachMuon = new QLineEdit();
    formLayout_muonSach->addRow("Mã thẻ độc giả:", lineEdit_maTheMuon);
    formLayout_muonSach->addRow("Mã sách:", lineEdit_maSachMuon);
    btn_muonSach = new QPushButton("Mượn sách");
    formLayout_muonSach->addRow(btn_muonSach);
    groupBox_muonSach->setLayout(formLayout_muonSach);

    // GroupBox: Trả sách
    groupBox_traSach = new QGroupBox("Trả sách", tab_muonTra);
    QFormLayout *formLayout_traSach = new QFormLayout();
    lineEdit_maTheTra = new QLineEdit();
    lineEdit_maSachTra = new QLineEdit();
    formLayout_traSach->addRow("Mã thẻ độc giả:", lineEdit_maTheTra);
    formLayout_traSach->addRow("Mã sách:", lineEdit_maSachTra);
    btn_traSach = new QPushButton("Trả sách");
    formLayout_traSach->addRow(btn_traSach);
    groupBox_traSach->setLayout(formLayout_traSach);

    // GroupBox: Sách đang mượn
    groupBox_sachDangMuon = new QGroupBox("Sách đang mượn", tab_muonTra);
    QVBoxLayout *layout_sachDangMuon = new QVBoxLayout();
    QHBoxLayout *layout_xemSach = new QHBoxLayout();
    lineEdit_maTheXem = new QLineEdit();
    btn_xemSachDangMuon = new QPushButton("Xem");
    layout_xemSach->addWidget(new QLabel("Mã thẻ độc giả:"));
    layout_xemSach->addWidget(lineEdit_maTheXem);
    layout_xemSach->addWidget(btn_xemSachDangMuon);
    tableWidget_sachDangMuon = new QTableWidget();
    tableWidget_sachDangMuon->setColumnCount(4);
    tableWidget_sachDangMuon->setHorizontalHeaderLabels({"Mã sách", "Tên sách", "Ngày mượn"});
    layout_sachDangMuon->addLayout(layout_xemSach);
    layout_sachDangMuon->addWidget(tableWidget_sachDangMuon);
    groupBox_sachDangMuon->setLayout(layout_sachDangMuon);

    layout_muonTra->addWidget(groupBox_muonSach);
    layout_muonTra->addWidget(groupBox_traSach);
    layout_muonTra->addWidget(groupBox_sachDangMuon);
    tabWidget->addTab(tab_muonTra, "Mượn/Trả sách");

    // --- Tab 4: Thống kê ---
    QWidget *tab_thongKe = new QWidget();
    QVBoxLayout *layout_thongKe = new QVBoxLayout(tab_thongKe);

    // GroupBox: Sách quá hạn
    groupBox_quaHan = new QGroupBox("Sách quá hạn", tab_thongKe);
    QVBoxLayout *layout_quaHan = new QVBoxLayout();
    tableWidget_quaHan = new QTableWidget();
    tableWidget_quaHan->setColumnCount(7);
    tableWidget_quaHan->setHorizontalHeaderLabels({"Mã thẻ", "Họ", "Tên", "Mã sách", "Tên sách", "Ngày mượn", "Số ngày quá hạn"});
    btn_inQuaHan = new QPushButton("In danh sách");
    layout_quaHan->addWidget(tableWidget_quaHan);
    layout_quaHan->addWidget(btn_inQuaHan);
    groupBox_quaHan->setLayout(layout_quaHan);

    // GroupBox: Top 10 sách
    groupBox_topSach = new QGroupBox("Top 10 sách mượn nhiều nhất", tab_thongKe);
    QVBoxLayout *layout_topSach = new QVBoxLayout();
    tableWidget_topSach = new QTableWidget();
    tableWidget_topSach->setColumnCount(3);
    tableWidget_topSach->setHorizontalHeaderLabels({"Mã sách", "Tên sách", "Số lượt mượn"});
    btn_inTopSach = new QPushButton("In top 10");
    layout_topSach->addWidget(tableWidget_topSach);
    layout_topSach->addWidget(btn_inTopSach);
    groupBox_topSach->setLayout(layout_topSach);

    layout_thongKe->addWidget(groupBox_quaHan);
    layout_thongKe->addWidget(groupBox_topSach);
    tabWidget->addTab(tab_thongKe, "Thống kê");

    // Áp dụng style sheet
    setStyleSheet(
        "QPushButton {"
        "    background-color: #4CAF50;"
        "    color: white;"
        "    border-radius: 5px;"
        "    padding: 5px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #45a049;"
        "}"
        "QTableWidget {"
        "    border: 1px solid #ccc;"
        "    gridline-color: #ccc;"
        "}"
        "QLineEdit {"
        "    border: 1px solid #ccc;"
        "    border-radius: 3px;"
        "    padding: 3px;"
        "}"
        "QGroupBox {"
        "    font: bold 12px;"
        "    margin-top: 10px;"
        "}"
        );

    // Validate
    ValidationHelper::validateString(lineEdit_ho);
    ValidationHelper::validateString(lineEdit_ten);
    ValidationHelper::validateString(lineEdit_isbn);
    ValidationHelper::validateString(lineEdit_tenSach);
    ValidationHelper::validateInt(lineEdit_soTrang, 1);
    ValidationHelper::validateString(lineEdit_tacGia);
    ValidationHelper::validateInt(lineEdit_namXuatBan, 0, 2025);
    ValidationHelper::validateString(lineEdit_timTenSach);
    ValidationHelper::validateInt(lineEdit_maTheMuon);
    ValidationHelper::validateString(lineEdit_maSachMuon);
    ValidationHelper::validateInt(lineEdit_maTheTra);
    ValidationHelper::validateString(lineEdit_maSachTra);

    // Kết nối signals và slots
    connect(btn_themDocGia, &QPushButton::clicked, this, &MainWindow::on_btn_themDocGia_clicked);
    connect(btn_inDanhSachDocGia, &QPushButton::clicked, this, &MainWindow::on_btn_inDanhSachDocGia_clicked);
    connect(btn_themDauSach, &QPushButton::clicked, this, &MainWindow::on_btn_themDauSach_clicked);
    connect(btn_locDauSach, &QPushButton::clicked, this, &MainWindow::on_btn_locDauSach_clicked);
    connect(btn_muonSach, &QPushButton::clicked, this, &MainWindow::on_btn_muonSach_clicked);
    connect(btn_traSach, &QPushButton::clicked, this, &MainWindow::on_btn_traSach_clicked);
    connect(btn_xemSachDangMuon, &QPushButton::clicked, this, &MainWindow::on_btn_xemSachDangMuon_clicked);
    connect(btn_inQuaHan, &QPushButton::clicked, this, &MainWindow::on_btn_inQuaHan_clicked);
    connect(btn_inTopSach, &QPushButton::clicked, this, &MainWindow::on_btn_inTopSach_clicked);
}

MainWindow::~MainWindow() {
}

// --- Slot implementations (to be filled with logic) ---
void MainWindow::on_btn_themDocGia_clicked() {
    // Logic thêm độc giả
    string ho = lineEdit_ho->text().toStdString();
    string ten = lineEdit_ten->text().toStdString();
    string phai = comboBox_phai->currentText().toStdString();

    if (ho.empty() || ten.empty()) {
        QMessageBox::warning(this, "Lỗi", "Vui lòng nhập đầy đủ họ và tên.");
        return;
    }

    TheDocGia theDocGia = TheDocGia(ho, ten, phai);

    qlTheDocGia.themTheDocGia(theDocGia);

    QMessageBox::information(this, "Thành công", "Thêm độc giả thành công.");
}

void MainWindow::on_btn_inDanhSachDocGia_clicked() {
    if (radioButton_theoTen->isChecked()) {
        qlTheDocGia.hienThiDanhSachDocGiaTheoTen(tableWidget_docGia);
    } else if (radioButton_theoMa->isChecked()) {
        qlTheDocGia.hienThiDanhSachDocGiaTheoMa(tableWidget_docGia);
    } else {
        // Không chọn gì => mặc định theo thứ tự thêm
        qlTheDocGia.hienThiDanhSachDocGia(tableWidget_docGia);
    }
}


void MainWindow::on_btn_themDauSach_clicked() {
    // Logic thêm đầu sách
    bool checkSoTrang, checkNamXuatBan;

    string ISBN = lineEdit_isbn->text().toStdString();
    string tenSach = lineEdit_tenSach->text().toStdString();
    int soTrang = lineEdit_soTrang->text().toInt(&checkSoTrang);
    string tacGia = lineEdit_tacGia->text().toStdString();
    int namXuatBan = lineEdit_namXuatBan->text().toInt(&checkNamXuatBan);
    string theLoai = comboBox_theLoai->currentText().toStdString();

    if (ISBN.empty() || tenSach.empty() || tacGia.empty() || theLoai.empty() || !checkSoTrang || !checkNamXuatBan) {
        QMessageBox::warning(this, "Lỗi", "Vui lòng nhập đầy đủ thông tin.");
        return;
    }

    DauSach dauSach = DauSach(ISBN, tenSach, soTrang, tacGia, namXuatBan, theLoai);

    try {
        qlDauSach.themDauSach(dauSach);
        QMessageBox::information(this, "Thành công", "Đã thêm đầu sách.");
    } catch (const char *msg) {
        QMessageBox::critical(this, "Lỗi", msg);
    }
}

void MainWindow::on_btn_locDauSach_clicked() {
    // Logic lọc đầu sách theo thể loại
    string ten = lineEdit_timTenSach->text().toStdString();
    string theLoai = comboBox_locTheLoai->currentText().toStdString();
    if (theLoai == "Tất cả") {
        theLoai = "";
    }

    qlDauSach.hienThiDanhSachDauSach(ten, theLoai, tableWidget_dauSach);
}

void MainWindow::on_btn_muonSach_clicked() {
    // Logic mượn sách
    bool checkMaTheMuon;

    int maTheMuon = lineEdit_maTheMuon->text().toInt(&checkMaTheMuon);
    string maSachMuon = lineEdit_maSachMuon->text().toStdString();
    if (!checkMaTheMuon || maSachMuon.empty()) {
        QMessageBox::warning(this, "Lỗi", "Vui lòng không để trống.");
        return;
    }

    TheDocGia *theDocGia = qlTheDocGia.timTheDocGia(maTheMuon);
    if (theDocGia == nullptr) {
        QMessageBox::warning(this, "Lỗi", "Không tìm thấy thẻ độc giả.");
        return;
    }

    Sach *sach = qlDauSach.timSachTheoMaSach(maSachMuon);
    if (sach == nullptr) {
        QMessageBox::warning(this, "Lỗi", "Không tìm thấy sách.");
        return;
    }
    if (sach->getTrangThai() == TrangThaiSach::DA_DUOC_MUON) {
        QMessageBox::warning(this, "Lỗi", "Sách đã được mượn.");
        return;
    }
    if (sach->getTrangThai() == TrangThaiSach::DA_THANH_LY) {
        QMessageBox::warning(this, "Lỗi", "Sách đã được thanh lý.");
        return;
    }

    string isbn = MaHelper::layISBNTheoMaSach(sach->getMaSach());
    DauSach *ds = qlDauSach.timTheoISBN(isbn);
    if (ds == nullptr) {
        QMessageBox::warning(this, "Lỗi", "Sách không thuộc đầu sách nào.");
        return;
    }

    MuonTra muonTra = MuonTra(maSachMuon);
    try {
        theDocGia->muonSach(muonTra);
        sach->setTrangThai(TrangThaiSach::DA_DUOC_MUON);
        ds->tangSoLanMuon();
        QMessageBox::information(this, "Thành công", "Đã mượn sách thành công.");
    } catch (const char *msg) {
        QMessageBox::critical(this, "Lỗi", msg);
    }
}

void MainWindow::on_btn_traSach_clicked() {
    // Logic trả sách
    bool checkMaTheTra;

    int maTheTra = lineEdit_maTheTra->text().toInt(&checkMaTheTra);
    string maSachTra = lineEdit_maSachTra->text().toStdString();
    if (!checkMaTheTra || maSachTra.empty()) {
        QMessageBox::warning(this, "Lỗi", "Vui lòng không để trống.");
        return;
    }

    TheDocGia *theDocGia = qlTheDocGia.timTheDocGia(maTheTra);
    if (theDocGia == nullptr) {
        QMessageBox::warning(this, "Lỗi", "Không tìm thấy thẻ độc giả.");
        return;
    }

    Sach *sach = qlDauSach.timSachTheoMaSach(maSachTra);
    if (sach == nullptr) {
        QMessageBox::warning(this, "Lỗi", "Không tìm thấy sách.");
        return;
    }
    if (sach->getTrangThai() == TrangThaiSach::CHO_MUON_DUOC) {
        QMessageBox::warning(this, "Lỗi", "Sách đã được trả.");
        return;
    }
    if (sach->getTrangThai() == TrangThaiSach::DA_THANH_LY) {
        QMessageBox::warning(this, "Lỗi", "Sách đã được thanh lý.");
        return;
    }

    try {
        theDocGia->traSach(maSachTra);
        sach->setTrangThai(TrangThaiSach::CHO_MUON_DUOC);
        QMessageBox::information(this, "Thành công", "Đã trả sách thành công.");
    } catch (const char *msg) {
        QMessageBox::critical(this, "Lỗi", msg);
    }
}

void MainWindow::on_btn_xemSachDangMuon_clicked() {
    // Logic xem sách đang mượn
    bool checkMaTheXem;

    int maTheXem = lineEdit_maTheXem->text().toInt(&checkMaTheXem);
    if (!checkMaTheXem) {
        QMessageBox::warning(this, "Lỗi", "Vui lòng không để trống.");
        return;
    }

    TheDocGia *theDocGia = qlTheDocGia.timTheDocGia(maTheXem);
    if (theDocGia == nullptr) {
        QMessageBox::warning(this, "Lỗi", "Không tìm thấy thẻ độc giả.");
        return;
    }

    qlTheDocGia.hienThiDanhSachSachDangMuon(*theDocGia, qlDauSach, tableWidget_sachDangMuon);
}

void MainWindow::on_btn_inQuaHan_clicked() {
    // Logic in danh sách quá hạn
}

void MainWindow::on_btn_inTopSach_clicked() {
    // Logic in top 10 sách
    qlDauSach.hienThiTop10Sach(tableWidget_topSach);
}
