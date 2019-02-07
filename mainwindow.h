#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QAxObject>
#include <QTableWidgetItem>
#include <QGroupBox>
#include <QLineEdit>
#include <QLabel>

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

#include <QTextStream>
#include <QSslError>
#include <QMessageBox>
#include <QPainter>

#include <QDesktopServices>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QFileDialog>
#include <QDebug>
#include <QStandardItemModel>
#include <QDate>

#include "spoiler.h"
#include "loader.h"

#include "Settings/settingsbase.h"
#include "Settings/settingspenny.h"
#include "Settings/settingsdateformat.h"
#include "aboutprogramdialog.h"
#include "excelexporthelper.h"
#include <exception>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    int DebitMin = 0;
    int DebitMax = 0;
    int DeadlinePeriodDays = 9;
    int DeadlinePeriodMonth = 1;
    int EarlyDelayPeriodDays = 1;
    int EarlyDelayPeriodMonth = 1;
    QString **arrCoefficient = nullptr;
    bool debt = true;
    bool IgnoreString = true;
    bool ConsiderPenny = true;
    QStringList JSONSettingsPathList;
    QStringList JSONSettingsPath(const QString &path);
    void saveAsCSV(QString filename);

private Q_SLOTS:
    void on_pushButton_clicked();
    void import(QString fileName, int sheetNumber=1);
    void on_pushButton_2_clicked();
    //    int ComparingDatesdd(const char *dataone, const char *datatwo);
    //    int ComparingDatesmm(QDate dataone, QDate datatwo);
    //    int DifferenceBetweenDtesmm(char *dataone, char *datatwo);
    void on_pushButton_4_clicked();
    void dataUploader(QString **arrKeyRate,int row_arr_KeyRate);
    void ChangedDate();
    void ChangedCelltabWidget (int row, int column);
    void linkWikipedia(const QString & url);
    void getSettings();

    //    void ChangedTextLineEdit(QString ValueText);
    void ChangedTextLineEdit_4(QString ValueText);
    void ChangedTextLineEdit_5(QString ValueText);
    void ChangedTextLineEdit_6(QString ValueText);
    void ChangedTextLineEdit_7(QString ValueText);
    void ChangedTextLineEdit_8(QString ValueText);
    void ChangedTextLineEdit_9(QString ValueText);
    void ChangedTextLineEdit_10(QString ValueText);

    void on_action_triggered();
    void on_action_2_triggered();
    void on_aboutprogram_triggered();

    void on_pushButton_export_to_CSV_clicked();
    void on_pushButton_export_to_xlsx_clicked();
    bool DetermineNumberORDate(QString data);

private:
    Ui::MainWindow *ui;
    QString File;
    Loader theLoader;
    QString **arrDuty;
    int end_Debit_row;
    QString searchData;
    QString searchEndDay;
    QString searchDebit;
    QString searchMonthDebit;
    QString searchCredit;
    QString searchMonthCredit;
    QString searchTotals;
    int column_data = -100;
    int intRowStart = -100;
    int intColStart = -100;
    QMessageBox msgBox;
    Spoiler* SpoilerKeywords = new Spoiler("Ключивые слова",900);
    QGroupBox *groupBox = new QGroupBox(tr("Обязательно указать ключевые слова, как в Excel документе. Они должны быть один в один похожи. (Учитываются пробелы.)"));
    QGridLayout *GLgroupBox = new QGridLayout;
    QLineEdit *lineEdit_10 = new QLineEdit(this);
    QLineEdit *lineEdit_9 = new QLineEdit(this);
    QLineEdit *lineEdit_8 = new QLineEdit(this);
    QLineEdit *lineEdit_7 = new QLineEdit(this);
    QLineEdit *lineEdit_6 = new QLineEdit(this);
    QLineEdit *lineEdit_5 = new QLineEdit(this);
    QLineEdit *lineEdit_4 = new QLineEdit(this);
    QLabel *label_10 = new QLabel("Дата");
    QLabel *label_9 = new QLabel("Где указан долг (Конец дня)");
    QLabel *label_8 = new QLabel("Последняя строчка,\n в которой заканчиваются\n перечисления дебетов,\n кредетов и месяцев.\n Её надо отметить как \"Итого:\",\n не забудьте \":\"");
    QLabel *label_7 = new QLabel("Месяцы оплаты");
    QLabel *label_6 = new QLabel("Где указан кредет");
    QLabel *label_5 = new QLabel("Месяцы зачисления\n дебета");
    QLabel *label_4 = new QLabel("Где указан дебет");
    QStringList DateFormatList;
    QDate ValidationDateVerification(QString Data, QString WhatIsDate = "без имени",bool msgBoxBool=true);
    QDate count_date_end;
    QJsonArray jsonArray;
    QStandardItemModel *model = new QStandardItemModel;
    QStandardItem *item;

    SettingsBase *Settings = nullptr;
    SettingsPenny *SettingsPennyDialog = nullptr;
    SettingsDateFormat *SettingsDateFormatDialog = nullptr;
    // конечный подсчёт с ключевыми ставками

    int count_row_Duty = 0;
    int count_row_KeyRate = 0;

    bool triggerDuty = false;
    bool triggerDate = false;

    int Coefficient_row=0;
    int begin_row = 0;

    double Debt =0;
    double sumDebt =0;
    double sumDebtAll =0;

    int countDay = 0;
    int daysToDate = 0;

    QString Coefficient_value;

    int TableConstruction(QDate d1, QDate d2, QString arrKeyRate_value, int i, bool triggerBegin = false, bool triggerEnd = false);

    // конечный подсчёт с ключевыми ставками
};

#endif // MAINWINDOW_H
