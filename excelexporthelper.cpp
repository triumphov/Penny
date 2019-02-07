#include "excelexporthelper.h"

#include <QDebug>

using namespace std;

ExcelExportHelper::ExcelExportHelper(bool closeExcelOnExit)
{
    m_closeExcelOnExit = closeExcelOnExit;
    m_excelApplication = nullptr;
    cell = nullptr;
    razmer = nullptr;
    rangec = nullptr;
    m_sheet = nullptr;
    m_sheets = nullptr;
    m_workbook = nullptr;
    m_workbooks = nullptr;
    m_excelApplication = nullptr;

    m_excelApplication = new QAxObject("Excel.Application", nullptr);//{00024500-0000-0000-C000-000000000046}
    if (m_excelApplication->isNull()!=true) {
        if (m_excelApplication == nullptr)
            throw invalid_argument("Failed to initialize interop with Excel (probably Excel is not installed)");

        //    m_excelApplication->dynamicCall("SetVisible(bool)", false); // hide excel
        //    m_excelApplication->setProperty("DisplayAlerts", 0); // disable alerts

        m_workbooks = m_excelApplication->querySubObject("Workbooks");
        m_workbook = m_workbooks->querySubObject("Add");
        if (m_workbook){
            m_sheets = m_workbook->querySubObject("Worksheets");
            m_sheet = m_sheets->querySubObject("Add");
            rangec = m_sheet->querySubObject( "Range(const QVariant&)",QVariant(QString("A:G")));
            // получаю указатель на строку
            razmer = rangec->querySubObject("Columns");
            // устанавливаю её размер.
            razmer->setProperty("ColumnWidth",15);
        }
    } else {
        msgBox.setText(QString("Установите Excel."));
        msgBox.exec();
    }
}

void ExcelExportHelper::SetCellValue(int lineIndex, int columnIndex, QString value)
{
    if(!m_sheet ) {
        qDebug() << Q_FUNC_INFO << "!m_sheet";
        return;
    }
    cell =  m_sheet->querySubObject("Cells(int,int)", lineIndex, columnIndex);
    if(!cell ) {
        qDebug() << Q_FUNC_INFO << "!cell";
        return;
    }
    cell->setProperty("Value",value);
}

void ExcelExportHelper::SaveAs(const QString& fileName)
{
    if (fileName == "")
        throw invalid_argument("'fileName' пустой!");
    if (fileName.contains("/"))
        throw invalid_argument("Символ '/' в 'fileName' не поддерживается Excel!");

    if (QFile::exists(fileName))
    {
        if (!QFile::remove(fileName))
        {
            throw new exception();
        }
    }

    m_workbook->dynamicCall("SaveAs (const QString&)", fileName);
}

ExcelExportHelper::~ExcelExportHelper()
{
    if (m_excelApplication != nullptr)
    {
        if (!m_closeExcelOnExit)
        {
            m_excelApplication->setProperty("DisplayAlerts", 1);
            m_excelApplication->dynamicCall("SetVisible(bool)", true);
        }

        if (m_workbook != nullptr && m_closeExcelOnExit)
        {
            m_workbook->dynamicCall("Close (Boolean)", true);
            m_excelApplication->dynamicCall("Quit (void)");
        }
    }
    delete cell;
    delete razmer;
    delete rangec;
    delete m_sheet;
    delete m_sheets;
    delete m_workbook;
    delete m_workbooks;
    delete m_excelApplication;
}
