#-------------------------------------------------
#
# Project created by QtCreator 2018-04-25T15:36:29
#
#-------------------------------------------------

QT       += core gui
QT       += axcontainer network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
RESOURCES += resources.qrc

TARGET = Penny
TEMPLATE = app

LIBS += -Llibeay32 -Lssleay32
# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

win32 {
    RC_FILE += icon/pennyAppIcon.rc
    OTHER_FILES += icon/pennyAppIcon.rc
    RC_ICONS += icon/penny.ico
}

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    ctkrangeslider.cpp \
    loader.cpp \
    spoiler.cpp \
    buttondeletecolumndelegate.cpp \
    Settings/settingsbase.cpp \
    Settings/settingspenny.cpp \
    Settings/settingsdateformat.cpp \
    aboutprogramdialog.cpp \
    excelexporthelper.cpp

HEADERS += \
    mainwindow.h \
    loader.h \
    ctkrangeslider.h \
    spoiler.h \
    buttondeletecolumndelegate.h \
    Settings/settingsbase.h \
    Settings/settingspenny.h \
    Settings/settingsdateformat.h \
    aboutprogramdialog.h \
    excelexporthelper.h

FORMS += \
        mainwindow.ui
