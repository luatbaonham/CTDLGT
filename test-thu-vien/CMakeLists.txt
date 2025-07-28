cmake_minimum_required(VERSION 3.16)

project(test-thu-vien VERSION 0.1 LANGUAGES CXX)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/model
    ${CMAKE_CURRENT_SOURCE_DIR}/view
    ${CMAKE_CURRENT_SOURCE_DIR}/controller
    ${CMAKE_CURRENT_SOURCE_DIR}/utils
)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# Nhóm các file theo thư mục MVC
set(MODEL_SOURCES
    model/enums.h
    model/constants.h
    model/sach.h
    model/dau_sach.h
    model/ngay.h
    model/muon_tra.h
    model/the_doc_gia.h
)

set(VIEW_SOURCES
    view/mainwindow.h
    view/mainwindow.cpp
    view/mainwindow.ui
    view/dialog_sua_doc_gia.h
    view/dialog_them_sach.h
    view/dialog_chi_tiet_sach.h
)

set(CONTROLLER_SOURCES
    controller/quan_ly_the_doc_gia.h
    controller/quan_ly_dau_sach.h
)

set(UTILS_SOURCES
    utils/string_helper.h
    utils/date_helper.h
    utils/validation_helper.h
    utils/ma_helper.h
)

set(PROJECT_SOURCES
    main.cpp
    ${MODEL_SOURCES}
    ${VIEW_SOURCES}
    ${CONTROLLER_SOURCES}
    ${UTILS_SOURCES}
)

# Tạo executable (phân biệt Qt5 vs Qt6)
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(test-thu-vien
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(test-thu-vien SHARED ${PROJECT_SOURCES})
    else()
        add_executable(test-thu-vien ${PROJECT_SOURCES})
    endif()
endif()

# Liên kết thư viện Qt
target_link_libraries(test-thu-vien PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Các thông tin bổ sung cho macOS/iOS (nếu cần)
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.test-thu-vien)
endif()

set_target_properties(test-thu-vien PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# Cài đặt
include(GNUInstallDirs)
install(TARGETS test-thu-vien
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Qt6: finalize
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(test-thu-vien)
endif()
