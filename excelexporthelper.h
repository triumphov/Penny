#ifndef EXCELEXPORTHELPER_H
#define EXCELEXPORTHELPER_H

#include <ActiveQt/qaxobject.h>
#include <ActiveQt/qaxbase.h>
#include <QString>

#include <exception>
#include <QString>
#include <QFile>
#include <stdexcept>
#include <QAxObject>
#include <QAxWidget>
#include <QMessageBox>

class ExcelExportHelper
{
public:
    ExcelExportHelper(const ExcelExportHelper& other) = delete;
    ExcelExportHelper& operator=(const ExcelExportHelper& other) = delete;

    ExcelExportHelper(bool closeExcelOnExit = false);
    void SetCellValue(int lineIndex, int columnIndex, QString value);
    void SaveAs(const QString& fileName);

    ~ExcelExportHelper();

private:
    QAxObject* m_excelApplication;
    QAxObject* m_workbooks;
    QAxObject* m_workbook;
    QAxObject* m_sheets;
    QAxObject* m_sheet;
    QAxObject* rangec;
    QAxObject* razmer;
    QAxObject* cell;
    bool m_closeExcelOnExit;
    QMessageBox msgBox;
};

#endif // EXCELEXPORTHELPER_H
