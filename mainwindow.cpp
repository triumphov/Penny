#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <string>

#include "stdlib.h"
#include "math.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    MainWindow::setWindowTitle(tr("Расчет пени"));
    //QIcon icon("icon/penny.ico");
    MainWindow::setWindowIcon(QIcon(":/icon/penny.png"));
    //    MainWindow::setWindowIcon(QIcon(QDir::current().absolutePath()+"/icon/penny.ico"));
    //    ui->action->setIcon(QIcon(QDir::current().absolutePath()+"/icon/settings.ico"));
    //    ui->action_2->setIcon(QIcon(QDir::current().absolutePath()+"/icon/calendar.ico"));
    //    ui->aboutprogram->setIcon(QIcon(QDir::current().absolutePath()+"/icon/penny.ico"));
    ui->progressBar->hide();
    GLgroupBox->addWidget(label_10,0,0);
    GLgroupBox->addWidget(lineEdit_10,0,1);
    GLgroupBox->addWidget(label_9,0,2);
    GLgroupBox->addWidget(lineEdit_9,0,3);
    GLgroupBox->addWidget(label_8,0,4,2,2);
    GLgroupBox->addWidget(lineEdit_8,2,4,1,2);

    GLgroupBox->addWidget(label_4,1,0);
    GLgroupBox->addWidget(lineEdit_4,1,1);
    GLgroupBox->addWidget(label_5,1,2);
    GLgroupBox->addWidget(lineEdit_5,1,3);
    GLgroupBox->addWidget(label_6,2,0);
    GLgroupBox->addWidget(lineEdit_6,2,1);
    GLgroupBox->addWidget(label_7,2,2);
    GLgroupBox->addWidget(lineEdit_7,2,3);

    groupBox->setLayout(GLgroupBox);
    QGridLayout GLKeywords ;
    GLKeywords.addWidget(groupBox);
    SpoilerKeywords->setStyleSheet(" padding: 5px; ");
    SpoilerKeywords->setContentLayout(GLKeywords);
    ui->verticalLayout->addWidget(SpoilerKeywords);

    ui->lineEdit->setDisabled(true);
    lineEdit_4->setText("Дебет");
    lineEdit_4->setStyleSheet("QLineEdit { background:#00ff00 }");
    connect(lineEdit_4,SIGNAL(textChanged(const QString &)),this,SLOT(ChangedTextLineEdit_4(QString)));
    lineEdit_5->setText("МесДебет");
    lineEdit_5->setStyleSheet("QLineEdit { background:darkGreen }");
    connect(lineEdit_5,SIGNAL(textChanged(const QString &)),this,SLOT(ChangedTextLineEdit_5(QString)));
    lineEdit_6->setText("Кредит");
    lineEdit_6->setStyleSheet("QLineEdit { background:yellow }");
    connect(lineEdit_6,SIGNAL(textChanged(const QString &)),this,SLOT(ChangedTextLineEdit_6(QString)));
    lineEdit_7->setText("МесКредит");
    lineEdit_7->setStyleSheet("QLineEdit { background:#808000 }");
    connect(lineEdit_7,SIGNAL(textChanged(const QString &)),this,SLOT(ChangedTextLineEdit_7(QString)));
    lineEdit_8->setText("Итого:");
    lineEdit_8->setStyleSheet("QLineEdit { background:gray }");
    connect(lineEdit_8,SIGNAL(textChanged(const QString &)),this,SLOT(ChangedTextLineEdit_8(QString)));
    lineEdit_9->setText("КонецДня");
    lineEdit_9->setStyleSheet("QLineEdit { background:darkGray }");
    connect(lineEdit_9,SIGNAL(textChanged(const QString &)),this,SLOT(ChangedTextLineEdit_9(QString)));
    lineEdit_10->setText("Дата");
    lineEdit_10->setStyleSheet("QLineEdit { background:lightGray }");
    connect(lineEdit_10,SIGNAL(textChanged(const QString &)),this,SLOT(ChangedTextLineEdit_10(QString)));

    searchDebit = lineEdit_4->text();
    searchMonthDebit = lineEdit_5->text();
    searchCredit = lineEdit_6->text();
    searchMonthCredit = lineEdit_7->text();
    searchTotals = lineEdit_8->text();
    searchEndDay = lineEdit_9->text();
    searchData = lineEdit_10->text();

    connect(&theLoader, SIGNAL(dataReadyRead(QString**,int)),this,SLOT(dataUploader(QString**,int)));

    ui->beg_data->setDate(QDate::currentDate().addYears(-2));
    ui->end_data->setDate(QDate::currentDate());

    connect(ui->beg_data,SIGNAL(dateChanged(const QDate)),this,SLOT(ChangedDate()));
    connect(ui->end_data,SIGNAL(dateChanged(const QDate)),this,SLOT(ChangedDate()));

    connect(ui->tableWidget,SIGNAL(cellChanged(int,int)),this,SLOT(ChangedCelltabWidget(int,int)));
    connect(ui->label_Wikipedia, SIGNAL(linkActivated(const QString &)), this, SLOT(linkWikipedia(const QString &)));

    getSettings();
}

MainWindow::~MainWindow()
{
    delete ui;
    if (SettingsPennyDialog) {
        SettingsPennyDialog->destroyed();
        delete SettingsPennyDialog;
    }
    if (SettingsDateFormatDialog) {
        SettingsDateFormatDialog->destroyed();
        delete SettingsDateFormatDialog;
    }

}

QStringList MainWindow::JSONSettingsPath(const QString &path)
{
    QDir dir(path);
    QStringList::Iterator it;
    QStringList nameFilter;
    nameFilter<<"*.json";
    QStringList files = dir.entryList(nameFilter, QDir::Files);
    for (int i=0;i<files.count();i++) {
        JSONSettingsPathList.append(QString(path+"/"+files.at(i)));
    }
    QStringList dirs = dir.entryList(QDir::Dirs);
    it = dirs.begin();
    while (it != dirs.end()) {
        if (*it != "." && *it != "..")
            MainWindow::JSONSettingsPath(path + "/" + *it);
        ++it;
    }
    return JSONSettingsPathList;
}

void MainWindow::getSettings() {
    QStringList Setting = MainWindow::JSONSettingsPath(QDir::currentPath());

    if (!SettingsPennyDialog) {SettingsPennyDialog = new SettingsPenny(Settings);}
    if (!SettingsDateFormatDialog) {SettingsDateFormatDialog = new SettingsDateFormat(Settings);}
    if (SettingsPennyDialog && SettingsDateFormatDialog) {
        for (int i = 0; i < Setting.size(); i++) {
            QFileInfo infoSettings(Setting.at(i));
            if (infoSettings.fileName()=="penny.json") {
                QString SettingsPath = infoSettings.absoluteFilePath();
                QFile loadFile(SettingsPath);
                if (!loadFile.open(QIODevice::ReadOnly)) {
                    QMessageBox::critical(this, tr("Ошибка"), tr("Нельзя открыть файл!"));
                    SettingsPennyDialog->DefaultValue();
                    MainWindow::getSettings();
                } else {
                    QByteArray SettingsData = loadFile.readAll();
                    QJsonDocument loadDoc(QJsonDocument::fromJson(SettingsData));

                    DebitMin = (loadDoc.object())["DebitMin"].toInt();
                    DebitMax = (loadDoc.object())["DebitMax"].toInt();

                    DeadlinePeriodDays = (loadDoc.object())["DeadlinePeriodDays"].toInt();
                    DeadlinePeriodMonth = (loadDoc.object())["DeadlinePeriodMonth"].toInt();
                    EarlyDelayPeriodDays = (loadDoc.object())["EarlyDelayPeriodDays"].toInt();
                    EarlyDelayPeriodMonth = (loadDoc.object())["EarlyDelayPeriodMonth"].toInt();

                    jsonArray = loadDoc["coefficient"].toArray();
                    arrCoefficient = new QString* [jsonArray.count()];

                    for (int i=0; i<jsonArray.count();i++)
                    {
                        arrCoefficient[i] = new QString[4];
                    }

                    foreach (const QJsonValue & value, jsonArray) {
                        QJsonObject obj = value.toObject();
                        arrCoefficient[obj["Id"].toInt()][0]= QString::number(obj["Id"].toInt());
                        arrCoefficient[obj["Id"].toInt()][1]= obj["ValueIndex"].toString();
                        arrCoefficient[obj["Id"].toInt()][2]= obj["PeriodDays"].toString();
                        arrCoefficient[obj["Id"].toInt()][3]= obj["PeriodMonth"].toString();
                    }

                    debt = (loadDoc.object())["debt"].toBool();

                    IgnoreString = (loadDoc.object())["IgnoreString"].toBool();
                    ConsiderPenny = (loadDoc.object())["ConsiderPenny"].toBool();

                }
                qDebug()<<"Debit"<<DebitMin<<DebitMax;
            } else if (infoSettings.fileName()=="formatDate.json") {
                QString SettingsPath = infoSettings.absoluteFilePath();
                QFile loadFile(SettingsPath);
                if (!loadFile.open(QIODevice::ReadOnly)) {
                    QMessageBox::critical(this, tr("Ошибка"), tr("Нельзя открыть файл!"));
                    SettingsDateFormatDialog->DefaultValue();
                    MainWindow::getSettings();
                } else {
                    QByteArray SettingsData = loadFile.readAll();
                    QJsonDocument loadDoc(QJsonDocument::fromJson(SettingsData));
                    QJsonArray jsonArrayFormat;
                    jsonArrayFormat = loadDoc["Format"].toArray();

                    //jsonArrayFormat.fromStringList(DateFormatList);
                    foreach (const QJsonValue & value, jsonArrayFormat) {
                        QJsonObject obj = value.toObject();
                        DateFormatList.append(obj["dateFormat"].toString());
                    }
                }
            }
        }
    }
}

void MainWindow::linkWikipedia(const QString & url) {
    QDesktopServices::openUrl(QUrl("https://ru.wikipedia.org/wiki/%D0%9A%D0%BB%D1%8E%D1%87%D0%B5%D0%B2%D0%B0%D1%8F_%D1%81%D1%82%D0%B0%D0%B2%D0%BA%D0%B0_%D0%B2_%D0%A0%D0%BE%D1%81%D1%81%D0%B8%D0%B8"));
}

void MainWindow::ChangedCelltabWidget (int row, int column) {
    if (ui->tableWidget->item(row,column)->background().color() == Qt::magenta) {
        return;
    } else if (ui->tableWidget->model()->data(ui->tableWidget->model()->index(row,column)).toString()==lineEdit_4->text()) {
        ui->tableWidget->item(row,column)->setBackground(Qt::green);
        return;
    } else if (ui->tableWidget->model()->data(ui->tableWidget->model()->index(row,column)).toString()==lineEdit_5->text()) {
        ui->tableWidget->item(row,column)->setBackground(Qt::darkGreen);
        return;
    } else if (ui->tableWidget->model()->data(ui->tableWidget->model()->index(row,column)).toString()==lineEdit_6->text()) {
        ui->tableWidget->item(row,column)->setBackground(Qt::yellow);
        return;
    } else if (ui->tableWidget->model()->data(ui->tableWidget->model()->index(row,column)).toString()==lineEdit_7->text()) {
        ui->tableWidget->item(row,column)->setBackground(Qt::darkYellow);
        return;
    } else if (ui->tableWidget->model()->data(ui->tableWidget->model()->index(row,column)).toString()==lineEdit_8->text()) {
        ui->tableWidget->item(row,column)->setBackground(Qt::gray);
        return;
    } else if (ui->tableWidget->model()->data(ui->tableWidget->model()->index(row,column)).toString()==lineEdit_9->text()) {
        ui->tableWidget->item(row,column)->setBackground(Qt::darkGray);
        return;
    } else if (ui->tableWidget->model()->data(ui->tableWidget->model()->index(row,column)).toString()==lineEdit_10->text()) {
        ui->tableWidget->item(row,column)->setBackground(Qt::lightGray);
        return;
    } else {
        ui->tableWidget->item(row,column)->setBackground(Qt::white);
        return;
        //        msgBox.setText(QString("Вы изменили ячйку %1,%2, на значение: %3").arg(row).arg(column).arg(ui->tableWidget->model()->data(ui->tableWidget->model()->index(row,column)).toString()));
        //        msgBox.exec();
        //        return;
    }
}

void MainWindow::ChangedTextLineEdit_4(QString ValueText)
{
    bool colorBool = false;
    searchDebit = lineEdit_4->text();
    if (intRowStart != -100 && intColStart != -100) {
        for (int row=intRowStart ; row < ui->tableWidget->rowCount(); row++) {
            for (int col=intColStart ; col < ui->tableWidget->columnCount(); col++) {
                if (ui->tableWidget->item(row-1,col-1)) {
                    if(ui->tableWidget->model()->data(ui->tableWidget->model()->index(row-1,col-1)).toString()==ValueText) {
                        ui->tableWidget->item(row-1,col-1)->setBackground(Qt::green);
                        colorBool = true;
                    }
                }
            }
        }
        if (colorBool==false) {
            for (int row=intRowStart ; row < ui->tableWidget->rowCount(); row++) {
                for (int col=intColStart ; col < ui->tableWidget->columnCount(); col++) {
                    if (ui->tableWidget->item(row-1,col-1)) {
                        if(ui->tableWidget->item(row-1,col-1)->background().color() == Qt::green) {
                            ui->tableWidget->item(row-1,col-1)->setBackground(Qt::white);
                            return;
                        }
                    }
                }
            }
        }
        return;
    }
}
void MainWindow::ChangedTextLineEdit_5(QString ValueText)
{
    bool colorBool = false;
    searchMonthDebit = lineEdit_5->text();
    if (intRowStart != -100 && intColStart != -100) {
        for (int row=intRowStart ; row < ui->tableWidget->rowCount(); row++) {
            for (int col=intColStart ; col < ui->tableWidget->columnCount(); col++) {
                if (ui->tableWidget->item(row-1,col-1)) {
                    if(ui->tableWidget->model()->data(ui->tableWidget->model()->index(row-1,col-1)).toString()==ValueText) {
                        ui->tableWidget->item(row-1,col-1)->setBackground(Qt::darkGreen);
                        colorBool = true;
                    }
                }
            }
        }
        if (colorBool==false) {
            for (int row=intRowStart ; row < ui->tableWidget->rowCount(); row++) {
                for (int col=intColStart ; col < ui->tableWidget->columnCount(); col++) {
                    if (ui->tableWidget->item(row-1,col-1)) {
                        if(ui->tableWidget->item(row-1,col-1)->background().color() == Qt::darkGreen) {
                            ui->tableWidget->item(row-1,col-1)->setBackground(Qt::white);
                            return;
                        }
                    }
                }
            }
        }
        return;
    }
}
void MainWindow::ChangedTextLineEdit_6(QString ValueText)
{
    bool colorBool = false;
    searchCredit = lineEdit_6->text();
    if (intRowStart != -100 && intColStart != -100) {
        for (int row=intRowStart ; row < ui->tableWidget->rowCount(); row++) {
            for (int col=intColStart ; col < ui->tableWidget->columnCount(); col++) {
                if (ui->tableWidget->item(row-1,col-1)) {
                    if(ui->tableWidget->model()->data(ui->tableWidget->model()->index(row-1,col-1)).toString()==ValueText) {
                        ui->tableWidget->item(row-1,col-1)->setBackground(Qt::yellow);
                        colorBool = true;
                    }
                }
            }
        }
        if (colorBool==false) {
            for (int row=intRowStart ; row < ui->tableWidget->rowCount(); row++) {
                for (int col=intColStart ; col < ui->tableWidget->columnCount(); col++) {
                    if (ui->tableWidget->item(row-1,col-1)) {
                        if(ui->tableWidget->item(row-1,col-1)->background().color() == Qt::yellow) {
                            ui->tableWidget->item(row-1,col-1)->setBackground(Qt::white);
                            return;
                        }
                    }
                }
            }
        }
        return;
    }
}

void MainWindow::ChangedTextLineEdit_7(QString ValueText)
{
    bool colorBool = false;
    searchMonthCredit = lineEdit_7->text();
    if (intRowStart != -100 && intColStart != -100) {
        for (int row=intRowStart ; row < ui->tableWidget->rowCount(); row++) {
            for (int col=intColStart ; col < ui->tableWidget->columnCount(); col++) {
                if (ui->tableWidget->item(row-1,col-1)) {
                    if(ui->tableWidget->model()->data(ui->tableWidget->model()->index(row-1,col-1)).toString()==ValueText) {
                        ui->tableWidget->item(row-1,col-1)->setBackground(Qt::darkYellow);
                        colorBool = true;
                    }
                }
            }
        }
        if (colorBool==false) {
            for (int row=intRowStart ; row < ui->tableWidget->rowCount(); row++) {
                for (int col=intColStart ; col < ui->tableWidget->columnCount(); col++) {
                    if (ui->tableWidget->item(row-1,col-1)) {
                        if(ui->tableWidget->item(row-1,col-1)->background().color() == Qt::darkYellow) {
                            ui->tableWidget->item(row-1,col-1)->setBackground(Qt::white);
                            return;
                        }
                    }
                }
            }
        }
        return;
    }
}

void MainWindow::ChangedTextLineEdit_8(QString ValueText)
{
    bool colorBool = false;
    searchTotals = lineEdit_8->text();
    if (intRowStart != -100 && intColStart != -100) {
        for (int row=intRowStart ; row < ui->tableWidget->rowCount(); row++) {
            for (int col=intColStart ; col < ui->tableWidget->columnCount(); col++) {
                if (ui->tableWidget->item(row-1,col-1)) {
                    if(ui->tableWidget->model()->data(ui->tableWidget->model()->index(row-1,col-1)).toString()==ValueText) {
                        ui->tableWidget->item(row-1,col-1)->setBackground(Qt::gray);
                        colorBool = true;
                    }
                }
            }
        }
        if (colorBool==false) {
            for (int row=intRowStart ; row < ui->tableWidget->rowCount(); row++) {
                for (int col=intColStart ; col < ui->tableWidget->columnCount(); col++) {
                    if (ui->tableWidget->item(row-1,col-1)) {
                        if(ui->tableWidget->item(row-1,col-1)->background().color() == Qt::gray) {
                            ui->tableWidget->item(row-1,col-1)->setBackground(Qt::white);
                            return;
                        }
                    }
                }
            }
        }
        return;
    }
}
void MainWindow::ChangedTextLineEdit_9(QString ValueText)
{
    bool colorBool = false;
    searchEndDay = lineEdit_9->text();
    if (intRowStart != -100 && intColStart != -100) {
        for (int row=intRowStart ; row < ui->tableWidget->rowCount(); row++) {
            for (int col=intColStart ; col < ui->tableWidget->columnCount(); col++) {
                if (ui->tableWidget->item(row-1,col-1)) {
                    if(ui->tableWidget->model()->data(ui->tableWidget->model()->index(row-1,col-1)).toString()==ValueText) {
                        ui->tableWidget->item(row-1,col-1)->setBackground(Qt::darkGray);
                        colorBool = true;
                    }
                }
            }
        }
        if (colorBool==false) {
            for (int row=intRowStart ; row < ui->tableWidget->rowCount(); row++) {
                for (int col=intColStart ; col < ui->tableWidget->columnCount(); col++) {
                    if (ui->tableWidget->item(row-1,col-1)) {
                        if(ui->tableWidget->item(row-1,col-1)->background().color() == Qt::darkGray) {
                            ui->tableWidget->item(row-1,col-1)->setBackground(Qt::white);
                            return;
                        }
                    }
                }
            }
        }
        return;
    }
}

void MainWindow::ChangedTextLineEdit_10(QString ValueText)
{
    bool colorBool = false;
    searchData = lineEdit_10->text();
    if (intRowStart != -100 && intColStart != -100) {
        for (int row=intRowStart ; row < ui->tableWidget->rowCount(); row++) {
            for (int col=intColStart ; col < ui->tableWidget->columnCount(); col++) {
                if (ui->tableWidget->item(row-1,col-1)) {
                    if(ui->tableWidget->model()->data(ui->tableWidget->model()->index(row-1,col-1)).toString()==ValueText) {
                        ui->tableWidget->item(row-1,col-1)->setBackground(Qt::lightGray);
                        colorBool = true;
                    }
                }
            }
        }
        if (colorBool==false) {
            for (int row=intRowStart ; row < ui->tableWidget->rowCount(); row++) {
                for (int col=intColStart ; col < ui->tableWidget->columnCount(); col++) {
                    if (ui->tableWidget->item(row-1,col-1)) {
                        if(ui->tableWidget->item(row-1,col-1)->background().color() == Qt::lightGray) {
                            ui->tableWidget->item(row-1,col-1)->setBackground(Qt::white);
                            return;
                        }
                    }
                }
            }
        }
        return;
    }
}

//void MainWindow::ChangedTextLineEdit(QString ValueText) {
//    qDebug()<<"ChangedTextLineEdit";
//    QString LEobj = "LE";
//    QObject *obj = QObject::sender();
//    //    if (QLineEdit *LE = qobject_cast<QLineEdit *>(obj)) {
//    //        LEobj=LE->objectName();
//    //    }
//    disconnect( obj, SIGNAL(textChanged(const QString &)), nullptr, nullptr );
//    LEobj=obj->objectName();
//    if (LEobj=="lineEdit_4") {
//        searchDebit = lineEdit_4->text();
//    } else if (LEobj=="lineEdit_5") {
//        searchMonthDebit = lineEdit_5->text();
//    } else if (LEobj=="lineEdit_6") {
//        searchCredit = lineEdit_6->text();
//    } else if (LEobj=="lineEdit_7") {
//        searchMonthCredit = lineEdit_7->text();
//    } else if (LEobj=="lineEdit_8") {
//        searchTotals = lineEdit_8->text();
//    } else if (LEobj=="lineEdit_9") {
//        searchEndDay = lineEdit_9->text();
//    } else if (LEobj=="lineEdit_10") {
//        searchData = lineEdit_10->text();
//    }

//    if (intRowStart != -100 && intColStart != -100) {
//        for (int row=intRowStart ; row < ui->tableWidget->rowCount(); row++) {
//            for (int col=intColStart ; col < ui->tableWidget->columnCount(); col++) {
//                if (ui->tableWidget->item(row-1,col-1)) {
//                    if(ui->tableWidget->model()->data(ui->tableWidget->model()->index(row-1,col-1)).toString()==ValueText) {
//                        if (LEobj=="lineEdit_4") {
//                            ui->tableWidget->item(row-1,col-1)->setBackground(Qt::green);
//                            searchDebit = lineEdit_4->text();
//                            return;
//                        } else if (LEobj=="lineEdit_5") {
//                            ui->tableWidget->item(row-1,col-1)->setBackground(Qt::darkGreen);
//                            searchMonthDebit = lineEdit_5->text();
//                            return;
//                        } else if (LEobj=="lineEdit_6") {
//                            ui->tableWidget->item(row-1,col-1)->setBackground(Qt::yellow);
//                            searchCredit = lineEdit_6->text();
//                            return;
//                        } else if (LEobj=="lineEdit_7") {
//                            ui->tableWidget->item(row-1,col-1)->setBackground(Qt::darkYellow);
//                            searchMonthCredit = lineEdit_7->text();
//                            return;
//                        } else if (LEobj=="lineEdit_8") {
//                            ui->tableWidget->item(row-1,col-1)->setBackground(Qt::gray);
//                            searchTotals = lineEdit_8->text();
//                            return;
//                        } else if (LEobj=="lineEdit_9") {
//                            ui->tableWidget->item(row-1,col-1)->setBackground(Qt::darkGray);
//                            searchEndDay = lineEdit_9->text();
//                            return;
//                        } else if (LEobj=="lineEdit_10") {
//                            ui->tableWidget->item(row-1,col-1)->setBackground(Qt::lightGray);
//                            searchData = lineEdit_10->text();
//                            return;
//                        }
//                    }
//                }
//            }
//        }
//        return;
//    }
//}

void MainWindow::ChangedDate()
{
    bool colorBool = false;
    if (intRowStart != -100 && intColStart != -100) {
        for (int row=intRowStart ; row < ui->tableWidget->rowCount(); row++) {
            for (int col=intColStart ; col < ui->tableWidget->columnCount(); col++) {
                if(ui->tableWidget->item(row-1,col-1)) {
                    if (ui->tableWidget->item(row-1,col-1)->background().color() == Qt::magenta) {
                        ui->tableWidget->item(row-1,col-1)->setBackground(Qt::white);
                    }
                    if (column_data != -100) {
                        if (column_data == col-1) {
                            if(MainWindow::ValidationDateVerification(ui->tableWidget->item(row-1,col-1)->text(),"дата",false)>=ui->beg_data->date() && MainWindow::ValidationDateVerification(ui->tableWidget->item(row-1,col-1)->text(),"дата",false)<=ui->end_data->date()) {
                                colorBool = true;
                            } else {
                                colorBool = false;
                            }
                        }
                    }
                    if ( colorBool == true ) {
                        ui->tableWidget->item(row-1,col-1)->setBackground(Qt::magenta);
                    }
                }
            }
        }
    }
}

void MainWindow::import(QString fileName, int sheetNumber)
{
    bool colorBool = false;
    QAxObject* excel = new QAxObject( "Excel.Application", nullptr );
    if (excel->isNull()!=true) {
        ui->progressBar->show();
        QAxObject* workbooks = excel->querySubObject("Workbooks");
        QAxObject* workbook = workbooks->querySubObject("Open(const QString&)", fileName);
        QAxObject* sheets = workbook->querySubObject("Worksheets");
        //int sheetCount = sheets->dynamicCall("Count()").toInt();
        QAxObject* sheet = sheets->querySubObject("Item( int )", sheetNumber);

        // Find the cells that actually have content
        QAxObject* usedrange = sheet->querySubObject( "UsedRange");
        QAxObject * rows = usedrange->querySubObject("Rows");
        QAxObject * columns = usedrange->querySubObject("Columns");
        intRowStart = usedrange->property("Row").toInt();
        intColStart = usedrange->property("Column").toInt();
        int intCols = columns->property("Count").toInt();
        int intRows = rows->property("Count").toInt();
        ui->progressBar->setMinimum(intRowStart);
        ui->progressBar->setMaximum(intRowStart+intRows);

        // replicate the Excel content in the QTableWidget
        ui->tableWidget->setColumnCount(intColStart+intCols);
        ui->tableWidget->setRowCount(intRowStart+intRows);
        for (int row=intRowStart ; row < intRowStart+intRows ; row++) {
            for (int col=intColStart ; col < intColStart+intCols ; col++) {
                QAxObject* cell = sheet->querySubObject("Cells( int, int )", row, col);
                QVariant value = cell->dynamicCall("Value()");
                if (value.toString().isEmpty()) continue;
                QTableWidgetItem * twi = new QTableWidgetItem(value.toString());
                if(value.toString() == searchData) {
                    column_data = col-1;
                }
                if (column_data != -100) {
                    if (column_data == col-1) {
                        if(QDate::fromString(value.toString(),"dd.MM.yyyy")>=ui->beg_data->date() && QDate::fromString(value.toString(),"dd.MM.yyyy")<=ui->end_data->date()) {
                            colorBool = true;
                        } else {
                            colorBool = false;
                        }
                    }
                }
                if (value.toString()==lineEdit_4->text()) {
                    ui->tableWidget->setItem(row-1, col-1, twi);
                    ui->tableWidget->item(row-1, col-1)->setBackground(Qt::green);
                } else if (value.toString()==lineEdit_5->text()) {
                    ui->tableWidget->setItem(row-1, col-1, twi);
                    ui->tableWidget->item(row-1, col-1)->setBackground(Qt::darkGreen);
                } else if (value.toString()==lineEdit_6->text()) {
                    ui->tableWidget->setItem(row-1, col-1, twi);
                    ui->tableWidget->item(row-1, col-1)->setBackground(Qt::yellow);
                } else if (value.toString()==lineEdit_7->text()) {
                    ui->tableWidget->setItem(row-1, col-1, twi);
                    ui->tableWidget->item(row-1, col-1)->setBackground(Qt::darkYellow);
                }  else if (value.toString()==lineEdit_8->text()) {
                    ui->tableWidget->setItem(row-1, col-1, twi);
                    ui->tableWidget->item(row-1, col-1)->setBackground(Qt::gray);
                } else if (value.toString()==lineEdit_9->text()) {
                    ui->tableWidget->setItem(row-1, col-1, twi);
                    ui->tableWidget->item(row-1, col-1)->setBackground(Qt::darkGray);
                } else if (value.toString()==lineEdit_10->text()) {
                    ui->tableWidget->setItem(row-1, col-1, twi);
                    ui->tableWidget->item(row-1, col-1)->setBackground(Qt::lightGray);
                } else {
                    ui->tableWidget->setItem(row-1, col-1, twi);
                }
                if ( colorBool == true ) {
                    ui->tableWidget->item(row-1,col-1)->setBackground(Qt::magenta);
                }
            }
            ui->progressBar->setValue(row);
        }
        ui->progressBar->hide();
        //    QPalette p = ui->tableWidget->palette();
        //    p.setColor(QPalette::Text, Qt::red);
        //    ui->tableWidget->setPalette(p);
    } else {
        msgBox.setText(QString("Установите Excel."));
        msgBox.exec();
    }
}
void MainWindow::on_pushButton_clicked()
{
    File = QFileDialog::getOpenFileName(
                this,
                tr("Открыть файлы"),
                "C://",
                "Все файлы (*.*);;Файлы таблиц (* .xls * .xlsx * .xlsm * .xlsb)"
                );
    if (!File.isEmpty()) {
        ui->lineEdit->setText(File);
        import(File);
    } else {
        msgBox.setText(QString("Вы не выбрали файл!\nВыберите файл."));
        msgBox.exec();
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    QStringList Debit;
    QStringList MonthDebit;
    QStringList rowDebit;
    QStringList columnDebit;
    QStringList columnMonthDebit;
    QStringList Credit;
    QStringList MonthCredit;
    QStringList EndDay;

    int column_EndDay;
    //int column_data;
    int column_Debit;
    int column_MonthDebit;
    int column_Credit;
    int column_MonthCredit;
    int row_Totals;

    int row_arr_Debit=0;
    int row_arr_Credit=0;

    int row_count = ui->tableWidget->model()->rowCount();
    int column_count = ui->tableWidget->model()->columnCount();

    bool consilienceBool = false;

    if (row_count==0 && column_count==0) {
        msgBox.setText(QString("Выберите документ для расчётов, нажав на кнопку 'Импорт из Excel'."));
        msgBox.exec();
        return;
    }

    if (ui->beg_data->date()>ui->end_data->date() || ui->beg_data->date().year()==ui->end_data->date().year() && abs(ui->beg_data->date().month()-ui->end_data->date().month())<=2) {
        msgBox.setText(QString("Некорректный ввод дат.\n"
                               "Дата начала подсчёта должна быть позднее чем, на 2 месяца, даты конца подсчётов."));
        msgBox.exec();
        return;
    }

    for (int i=0; i<row_count;i++) {
        for (int j=0; j<column_count;j++) {
            if(ui->tableWidget->model()->data(ui->tableWidget->model()->index(i,j)).toString() == searchEndDay ) {
                column_EndDay=j;
                consilienceBool = true;
                break;
            } else if (consilienceBool == false && i==row_count-1 && j==column_count-1 && ui->tableWidget->model()->data(ui->tableWidget->model()->index(i,j)).toString() != searchEndDay ) {
                msgBox.setText(QString("Следите за цветовыми индикаторами.\n"
                                       "Когда все ключевые слова отмечены, только тогда можно продолжить вычисления.\n"
                                       "Этого слова нет в таблице: %1").arg(searchEndDay));
                msgBox.exec();
                return;
            }
        }
    }

    consilienceBool = false;

    for (int i=0; i<row_count;i++) {
        for (int j=0; j<column_count;j++) {
            if(ui->tableWidget->model()->data(ui->tableWidget->model()->index(i,j)).toString() == searchTotals ) {
                row_Totals=i;
                consilienceBool = true;
                break;
            } else if (consilienceBool == false && i==row_count-1 && j==column_count-1 && ui->tableWidget->model()->data(ui->tableWidget->model()->index(i,j)).toString() != searchTotals ) {
                msgBox.setText(QString("Следите за цветовыми индикаторами.\n"
                                       "Когда все ключевые слова отмечены, только тогда можно продолжить вычисления.\n"
                                       "Этого слова нет в таблице: %1").arg(searchTotals));
                msgBox.exec();
                return;
            }
        }
    }

    consilienceBool = false;

    for (int i=0; i<row_count;i++) {
        for (int j=0; j<column_count;j++) {
            if(ui->tableWidget->model()->data(ui->tableWidget->model()->index(i,j)).toString() == searchData ) {
                column_data = j;
                consilienceBool = true;
                break;
            } else if (consilienceBool == false && i==row_count-1 && j==column_count-1 && ui->tableWidget->model()->data(ui->tableWidget->model()->index(i,j)).toString() != searchData ) {
                msgBox.setText(QString("Следите за цветовыми индикаторами.\n"
                                       "Когда все ключевые слова отмечены, только тогда можно продолжить вычисления.\n"
                                       "Этого слова нет в таблице: %1").arg(searchData));
                msgBox.exec();
                return;
            }
        }
    }

    consilienceBool = false;

    for (int i=0; i<row_count;i++) {
        for (int j=0; j<column_count;j++) {
            if(ui->tableWidget->model()->data(ui->tableWidget->model()->index(i,j)).toString() == searchMonthDebit ) {
                column_MonthDebit = j;
                consilienceBool = true;
                break;
            } else if (consilienceBool == false && i==row_count-1 && j==column_count-1 && ui->tableWidget->model()->data(ui->tableWidget->model()->index(i,j)).toString() != searchMonthDebit ) {
                msgBox.setText(QString("Следите за цветовыми индикаторами.\n"
                                       "Когда все ключевые слова отмечены, только тогда можно продолжить вычисления.\n"
                                       "Этого слова нет в таблице: %1").arg(searchMonthDebit));
                msgBox.exec();
                return;
            }
        }
    }

    consilienceBool = false;
    bool DebitMaxBool = false;
    for (int i=0; i<row_count;i++) {
        for (int j=0; j<column_count;j++) {
            //if(ui->tableWidget->item(i,j)->text().isEmpty()) {
            if(ui->tableWidget->model()->data(ui->tableWidget->model()->index(i,j)).toString() == searchDebit ) {
                for(column_Debit=i+1;column_Debit<row_Totals;column_Debit++) {
                    if(!ui->tableWidget->model()->data(ui->tableWidget->model()->index(column_Debit,j)).toString().isEmpty() &&
                            ui->tableWidget->model()->data(ui->tableWidget->model()->index(column_Debit,j)).toDouble() < (double)DebitMax &&
                            ui->tableWidget->model()->data(ui->tableWidget->model()->index(column_Debit,j)).toDouble() > (double)DebitMin) {

                        // =========================================================================================//
                        QDate dataDebit = MainWindow::ValidationDateVerification(ui->tableWidget->model()->data(ui->tableWidget->model()->index(column_Debit,column_MonthDebit)).toString(),"дебит");
                        if (!dataDebit.isValid() && IgnoreString==true)
                        {

                        } else if (!dataDebit.isValid()&&IgnoreString==false) {
                            return;
                        } else {
                            Debit.append(ui->tableWidget->model()->data(ui->tableWidget->model()->index(column_Debit,j)).toString());
                            MonthDebit.append(ui->tableWidget->model()->data(ui->tableWidget->model()->index(column_Debit,column_MonthDebit)).toString());
                            rowDebit.append(QString::number(column_Debit));
                            columnDebit.append(QString::number(j));
                            columnMonthDebit.append(QString::number(column_MonthDebit));
                            if (ui->tableWidget->item(column_Debit,j)) {
                                ui->tableWidget->item(column_Debit,j)->setBackground(Qt::white);
                                ui->tableWidget->item(column_Debit,j)->setTextColor(Qt::black);
                                ui->tableWidget->item(column_Debit,column_MonthDebit)->setBackground(Qt::white);
                                ui->tableWidget->item(column_Debit,column_MonthDebit)->setTextColor(Qt::black);
                            }
                            row_arr_Debit++;
                            DebitMaxBool = true;
                        }

                        // =========================================================================================//

                    } else if (DebitMaxBool == false && row_Totals-1==column_Debit) {
                        msgBox.setText(QString("В исключения попали все значения Дебита.\n"
                                               "Исправите в 'Настройки->Настройки основных параметров расчёта пени'\n"
                                               "Максимальную сумму дебита, которую надо исключить."));
                        msgBox.exec();
                        return;
                    }
                }
                consilienceBool = true;
                break;
            } else if (consilienceBool == false && i==row_count-1 && j==column_count-1 && ui->tableWidget->model()->data(ui->tableWidget->model()->index(i,j)).toString() != searchDebit ) {
                msgBox.setText(QString("Следите за цветовыми индикаторами.\n"
                                       "Когда все ключевые слова отмечены, только тогда можно продолжить вычисления.\n"
                                       "Этого слова нет в таблице: %1").arg(searchDebit));
                msgBox.exec();
                return;
            }
            //}
        }
    }
    DebitMaxBool = false;
    consilienceBool = false;

    for (int i=0; i<row_count;i++) {
        for (int j=0; j<column_count;j++) {
            if(ui->tableWidget->model()->data(ui->tableWidget->model()->index(i,j)).toString() == searchMonthCredit ) {
                column_MonthCredit = j;
                consilienceBool = true;
                break;
            } else if (consilienceBool == false && i==row_count-1 && j==column_count-1 && ui->tableWidget->model()->data(ui->tableWidget->model()->index(i,j)).toString() != searchMonthCredit ) {
                msgBox.setText(QString("Следите за цветовыми индикаторами.\n"
                                       "Когда все ключевые слова отмечены, только тогда можно продолжить вычисления.\n"
                                       "Этого слова нет в таблице: %1").arg(searchMonthCredit));
                msgBox.exec();
                return;
            }
        }
    }

    consilienceBool = false;

    for (int i=0; i<row_count;i++) {
        for (int j=0; j<column_count;j++) {
            if(ui->tableWidget->model()->data(ui->tableWidget->model()->index(i,j)).toString() == searchCredit ) {
                for(column_Credit=i+1;column_Credit<row_Totals;column_Credit++) {
                    if(!ui->tableWidget->model()->data(ui->tableWidget->model()->index(column_Credit,j)).toString().isEmpty()) {
                        // =========================================================================================//
                        QDate dataCredit = MainWindow::ValidationDateVerification(ui->tableWidget->model()->data(ui->tableWidget->model()->index(column_Credit,column_MonthCredit)).toString(),"кредит");

                        if (!dataCredit.isValid()  && IgnoreString==true)
                        {

                        } else if (!dataCredit.isValid() && IgnoreString==false) {
                            return;
                        } else {
                            Credit.append(ui->tableWidget->model()->data(ui->tableWidget->model()->index(column_Credit,j)).toString());
                            MonthCredit.append(ui->tableWidget->model()->data(ui->tableWidget->model()->index(column_Credit,column_MonthCredit)).toString());
                            EndDay.append(ui->tableWidget->model()->data(ui->tableWidget->model()->index(column_Credit,column_EndDay)).toString());
                            row_arr_Credit++;
                        }
                        // =========================================================================================//
                    }
                }
                consilienceBool = true;
                break;
            } else if (consilienceBool == false && i==row_count-1 && j==column_count-1 && ui->tableWidget->model()->data(ui->tableWidget->model()->index(i,j)).toString() != searchCredit ) {
                msgBox.setText(QString("Следите за цветовыми индикаторами.\n"
                                       "Когда все ключевые слова отмечены, только тогда можно продолжить вычисления.\n"
                                       "Этого слова нет в таблице: %1").arg(searchCredit));
                msgBox.exec();
                return;
            }
        }
    }

    consilienceBool = false;

    QString **arrDebit = new QString* [row_arr_Debit];
    int column_arr_Debit = 5;
    for (int i=0; i<row_arr_Debit;i++)
    {
        arrDebit[i] = new QString[column_arr_Debit];
    }

    if (row_arr_Debit == MonthDebit.count() && row_arr_Debit == Debit.count())
    {
        for(int i=0;i<row_arr_Debit;i++)
        {
            arrDebit[i][0]=MonthDebit.at(i);
            arrDebit[i][1]=Debit.at(i);
            arrDebit[i][2]=rowDebit.at(i);
            arrDebit[i][3]=columnDebit.at(i);
            arrDebit[i][4]=columnMonthDebit.at(i);
        }
    } else {
        qDebug()<<"Значения не совпадают row_arr_Debit и MonthDebit.count() или row_arr_Debit и Debit.count()";
    }

    QString **arrCredit = new QString* [row_arr_Credit];
    int column_arr_Credit = 3;
    for (int i=0; i<row_arr_Credit;i++)
    {
        arrCredit[i] = new QString[column_arr_Credit];
    }

    if (row_arr_Credit == MonthCredit.count() && row_arr_Credit == Credit.count()) {
        for(int i=0;i<row_arr_Credit;i++)
        {
            arrCredit[i][0]=MonthCredit.at(i);
            arrCredit[i][1]=Credit.at(i);
            arrCredit[i][2]=EndDay.at(i);
        }
    } else {
        qDebug()<<"Значения не совпадают row_arr_Credit и MonthCredit.count() или row_arr_Credit и Credit.count()";
    }

    int begin_Debit_row=0;
    end_Debit_row=0;
    int answer = 1;
    bool triggerDebit =false;
    for(int i=0;i<row_arr_Debit;i++) {
        if ( triggerDebit == false ) {
            //answer = ComparingDatesmm(ui->beg_data->date(),QDate::fromString(arrDebit[i][0], "MM.yyyy"));
            QDate QTdataone = ui->beg_data->date();
            QDate QTdatatwo = MainWindow::ValidationDateVerification(arrDebit[i][0],"дебит");
            if (!QTdatatwo.isValid())
            {
                return;
            }
            answer = QTdataone > QTdatatwo ? QTdataone.month() == QTdatatwo.month() && QTdataone.year() == QTdatatwo.year() ? 0 : 1 : 2;
            if(answer == 0 || answer == 2) {
                begin_Debit_row = i;
                triggerDebit = true;
                answer = 1;
            }
        } else if (i == row_arr_Debit-1) {
            end_Debit_row = i;
            triggerDebit = false;
            answer = 1;
            break;
        } else if (triggerDebit == true) {
            //answer = ComparingDatesmm(ui->end_data->date(),QDate::fromString(arrDebit[i][0], "MM.yyyy"));
            QDate QTdataone = ui->end_data->date();
            QDate QTdatatwo = MainWindow::ValidationDateVerification(arrDebit[i][0],"дебит");
            if (!QTdatatwo.isValid()) {return;}
            if (QTdataone.month() < QTdatatwo.month() && QTdataone.year() <= QTdatatwo.year()) {
                answer = 0;
            } else if (QTdataone.month() == QTdatatwo.month() && QTdataone.year() == QTdatatwo.year()) {
                answer = 2;
            } else {
                answer = 1;
            }
            if (answer == 0 || answer == 2) {
                end_Debit_row = i;
                triggerDebit = false;
                answer = 1;
                break;
            }
        } else {
            msgBox.setText("Что-то пошло не так. Период дебета не определился.");
            msgBox.exec();
            return;
        }
    }

    int begin_Credit_row=0;
    int end_Credit_row=0;
    QDate from_date = ui->beg_data->date();
    QDate to_date = ui->end_data->date();
    QDate current_date;

    for(int i=0;i<row_arr_Credit;i++) {
        current_date = MainWindow::ValidationDateVerification(arrCredit[i][0],"кредит");
        if (!current_date.isValid()) {return;}
        if (triggerDebit == false && current_date >= from_date) {
            begin_Credit_row = i;
            triggerDebit = true;
        }
        if (triggerDebit == true && current_date > to_date) {
            end_Credit_row = i-1;
            triggerDebit = false;
            break;
        }
        if(triggerDebit == true && i == row_arr_Credit-1 && current_date <= to_date) {
            end_Credit_row = i;
            triggerDebit = false;
            break;
        }
    }

    arrDuty = new QString* [row_arr_Debit];
    int column_arr_Duty = 3;
    for (int i=0; i<row_arr_Debit;i++)
    {
        arrDuty[i] = new QString[column_arr_Duty];
    }

    qDebug()<<"+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++";
    double amountDebt =0;
    double amountCredit =0;
    QDate DataDebit;
    QDate DataCredit;
    int n = begin_Credit_row;
    int answerDifference;
    qDebug()<<"begin_Credit_row"<<n;
    if(!arrCredit[n][1].isEmpty()) {
        amountCredit = arrCredit[n][1].toDouble();
    }
    double resultDebt = 0;

    DataDebit = MainWindow::ValidationDateVerification(arrDebit[end_Debit_row][0],"дебит");
    DataCredit = MainWindow::ValidationDateVerification(arrCredit[end_Credit_row][0],"кредит");
    if (DataDebit>=DataCredit && DataDebit<=to_date) {
        count_date_end = DataDebit;
    } else if (DataCredit>=DataDebit && DataCredit<=to_date) {
        count_date_end = DataCredit;
    } else if (to_date<=DataDebit && to_date<=DataCredit) {
        count_date_end = to_date;
    }
    ui->progressBar->show();
    ui->progressBar->setMinimum(begin_Debit_row);
    ui->progressBar->setMaximum(end_Debit_row);
    for(int m=begin_Debit_row; m<=end_Debit_row; m++) {

        DataDebit = MainWindow::ValidationDateVerification(arrDebit[m][0],"дебит");
        if (!DataDebit.isValid()) {return;}
        if (n<=end_Credit_row) {
            DataCredit = MainWindow::ValidationDateVerification(arrCredit[n][0],"кредит");
            if (!DataCredit.isValid()) {return;}
        }

        //        if (DataDebit.year()==DataCredit.year()) {
        //            answerDifference = DataDebit.month() - DataCredit.month();
        //        } else if (DataDebit.year()<DataCredit.year()) {
        //            answerDifference = DataDebit.month() - DataCredit.month();
        //        } else {
        //            qDebug()<<"year"<<DataDebit.year()<<DataCredit.year();
        //            msgBox.setText("Что-то пошло не так. Год дебета не может быть больше года кредита.");
        //            msgBox.exec();
        //            return;
        //        }

        answerDifference = DataDebit.month() - DataCredit.month();


        //if (){}

        if (abs(answerDifference)>=2) {
            amountDebt += arrDebit[m][1].toDouble();
            arrDuty[m][0]=arrDebit[m][0];
            arrDuty[m][1]=arrDebit[m][1];
            if (debt==true) {
                arrDuty[m][2]=count_date_end.toString("dd.MM.yyyy");
            } else {
                resultDebt = amountCredit - amountDebt;
                if (resultDebt>=0) {
                    if (n<=end_Credit_row) {
                        arrDuty[m][2]=arrCredit[n][0];
                    } else {
                        arrDuty[m][2]=count_date_end.toString("dd.MM.yyyy");
                    }
                } else if (resultDebt<0) {
                    if (n<=end_Credit_row) {
                        int j = n;
                        j++;
                        while (j<=end_Credit_row && resultDebt+arrCredit[j][1].toDouble()<0) {
                            if (resultDebt+arrCredit[j][1].toDouble()<0 && j==end_Credit_row) {
                                arrDuty[m][2]=count_date_end.toString("dd.MM.yyyy");
                            } else {
                                arrDuty[m][2]=arrCredit[j][0];
                            }
                            j++;
                        }
                    } else {
                        arrDuty[m][2]=count_date_end.toString("dd.MM.yyyy");
                    }
                }
            }
            if (ui->tableWidget->item(arrDebit[m][2].toInt(),arrDebit[m][3].toInt())) {
                ui->tableWidget->item(arrDebit[m][2].toInt(),arrDebit[m][3].toInt())->setBackground(Qt::white);
                ui->tableWidget->item(arrDebit[m][2].toInt(),arrDebit[m][3].toInt())->setTextColor(Qt::red);
                ui->tableWidget->item(arrDebit[m][2].toInt(),arrDebit[m][4].toInt())->setBackground(Qt::white);
                ui->tableWidget->item(arrDebit[m][2].toInt(),arrDebit[m][4].toInt())->setTextColor(Qt::red);
            }
        } else if (abs(answerDifference)==1) {
            amountDebt += arrDebit[m][1].toDouble();
            resultDebt = amountCredit- amountDebt;
            if (resultDebt>=0) {
                qDebug()<<"answerDifference = "<<answerDifference<<"Долг закрыт"<<"resultDebt="<<resultDebt;
            } else if (resultDebt<0) {
                arrDuty[m][0]=arrDebit[m][0];
                if (resultDebt<0) {
                    if (-resultDebt>arrDebit[m][1].toDouble()) {
                        arrDuty[m][1] = arrDebit[m][1];
                    } else {
                        arrDuty[m][1] = QString::number(-resultDebt);
                    }
                } else {
                    arrDuty[m][1]= QString::number(resultDebt);
                }
                if (debt==true) {
                    arrDuty[m][2]=count_date_end.toString("dd.MM.yyyy");
                } else {
                    resultDebt = amountCredit - amountDebt;
                    if (resultDebt>=0) {
                        if (n<=end_Credit_row) {
                            arrDuty[m][2]=arrCredit[n][0];
                        } else {
                            arrDuty[m][2]=count_date_end.toString("dd.MM.yyyy");
                        }
                    } else if (resultDebt<0) {
                        if (n<=end_Credit_row) {
                            int j = n;
                            j++;
                            while (j<=end_Credit_row && resultDebt+arrCredit[j][1].toDouble()<0) {
                                if (resultDebt+arrCredit[j][1].toDouble()<0 && j==end_Credit_row) {
                                    arrDuty[m][2]=count_date_end.toString("dd.MM.yyyy");
                                } else {
                                    arrDuty[m][2]=arrCredit[j][0];
                                }
                                j++;
                            }
                        } else {
                            arrDuty[m][2]=count_date_end.toString("dd.MM.yyyy");
                        }
                    }
                }
                if (ui->tableWidget->item(arrDebit[m][2].toInt(),arrDebit[m][3].toInt())) {
                    ui->tableWidget->item(arrDebit[m][2].toInt(),arrDebit[m][3].toInt())->setBackground(Qt::white);
                    ui->tableWidget->item(arrDebit[m][2].toInt(),arrDebit[m][3].toInt())->setTextColor(Qt::red);
                    ui->tableWidget->item(arrDebit[m][2].toInt(),arrDebit[m][4].toInt())->setBackground(Qt::white);
                    ui->tableWidget->item(arrDebit[m][2].toInt(),arrDebit[m][4].toInt())->setTextColor(Qt::red);
                }
            }
        } else if (answerDifference==0) {
            amountDebt += arrDebit[m][1].toDouble();
            resultDebt = amountCredit - amountDebt;

            if (arrDebit[m][1].toDouble() == arrCredit[n][1].toDouble() && debt==false) {
                qDebug()<<"answerDifference = "<<answerDifference<<"Долг закрыт";
            } else if (resultDebt>=0) {
                qDebug()<<"answerDifference = "<<answerDifference<<"Долг закрыт";
            } else if (resultDebt<0) {
                arrDuty[m][0]=arrDebit[m][0];
                if (resultDebt<0) {
                    if (-resultDebt>arrDebit[m][1].toDouble()) {
                        arrDuty[m][1] = arrDebit[m][1];
                    } else {
                        arrDuty[m][1] = QString::number(-resultDebt);
                    }
                } else {
                    arrDuty[m][1]= QString::number(resultDebt);
                }
                if (debt==true) {
                    arrDuty[m][2]=count_date_end.toString("dd.MM.yyyy");
                } else {
                    resultDebt = amountCredit - amountDebt;
                    if (resultDebt>=0) {
                        if (n<=end_Credit_row) {
                            arrDuty[m][2]=arrCredit[n][0];
                        } else {
                            arrDuty[m][2]=count_date_end.toString("dd.MM.yyyy");
                        }
                    } else if (resultDebt<0) {
                        if (n<=end_Credit_row) {
                            int j = n;
                            j++;
                            while (j<=end_Credit_row && resultDebt+arrCredit[j][1].toDouble()<0) {
                                if (resultDebt+arrCredit[j][1].toDouble()<0 && j==end_Credit_row) {
                                    arrDuty[m][2]=count_date_end.toString("dd.MM.yyyy");
                                } else {
                                    arrDuty[m][2]=arrCredit[j][0];
                                }
                                j++;
                            }
                        } else {
                            arrDuty[m][2]=count_date_end.toString("dd.MM.yyyy");
                        }
                    }
                }
                if (ui->tableWidget->item(arrDebit[m][2].toInt(),arrDebit[m][3].toInt())) {
                    ui->tableWidget->item(arrDebit[m][2].toInt(),arrDebit[m][3].toInt())->setBackground(Qt::white);
                    ui->tableWidget->item(arrDebit[m][2].toInt(),arrDebit[m][3].toInt())->setTextColor(Qt::red);
                    ui->tableWidget->item(arrDebit[m][2].toInt(),arrDebit[m][4].toInt())->setBackground(Qt::white);
                    ui->tableWidget->item(arrDebit[m][2].toInt(),arrDebit[m][4].toInt())->setTextColor(Qt::red);
                }
            }
            amountCredit=0;
            amountDebt=0;
            n++;
            if (n<=end_Credit_row) {
                amountCredit +=arrCredit[n][1].toDouble();
            }
        }











        ui->progressBar->setValue(m);
    }
    qDebug()<<"+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++";

    model->clear();
    ui->tableView->setModel(model);
    QDate yearDate_from;
    QDate yearDate_to;
    //Заголовки столбцов
    QStringList horizontalHeader;
    horizontalHeader.append("Дата начисления");
    horizontalHeader.append("Долг");
    horizontalHeader.append("Дата погашения");
    int row = 0;
    bool rowBool = false;
    ui->progressBar->setMinimum(row);
    ui->progressBar->setMaximum(row_arr_Debit);
    for( int i=0; i<row_arr_Debit; i++ ) {
        for( int j=0; j<3; j++ ) {
            if (j==0) {
                if(!arrDuty[i][j].isEmpty()) {
                    yearDate_from = MainWindow::ValidationDateVerification(arrDuty[i][j],"год");
                    if (!yearDate_from.isValid()) {return;}
                    if (i==0) {
                        item = new QStandardItem(yearDate_from.toString("yyyy"));
                        item->setTextAlignment(Qt::AlignCenter);
                        model->setItem(row, 1, item);
                        yearDate_to = yearDate_from;
                        row++;
                    } else {
                        if(yearDate_from.year()>yearDate_to.year()) {
                            item = new QStandardItem(yearDate_from.toString("yyyy"));
                            item->setTextAlignment(Qt::AlignCenter);
                            model->setItem(row, 1, item);
                            yearDate_to = yearDate_from;
                            row++;
                        }
                    }
                }
            }

            if (j==1) {
                item = new QStandardItem(arrDuty[i][j].replace(".",","));
            } else {
                item = new QStandardItem(arrDuty[i][j].replace(",","."));
            }

            if (!item->text().isEmpty()) {
                model->setItem(row, j, item);
                rowBool = true;
            } else {
                rowBool = false;
            }
        }
        if (rowBool==true) {row++;}
        ui->progressBar->setValue(row);
    }
    model->setHorizontalHeaderLabels(horizontalHeader);
    ui->tableView->setModel(model);
    for (int c = 0; c < ui->tableView->horizontalHeader()->count(); ++c)
    {
        ui->tableView->horizontalHeader()->setSectionResizeMode(c, QHeaderView::Stretch);
    }
    ui->progressBar->hide();
    ui->tabWidget->setCurrentIndex(1);
}

//int MainWindow::DifferenceBetweenDtesmm(char *dataone, char *datatwo) {
//    int mm1, mm2, yy1, yy2;
//    int i;
//    sscanf(dataone,"%d.%d", &mm1, &yy1);
//    sscanf(datatwo,"%d.%d", &mm2, &yy2);
//    i=abs(mm2-mm1+(yy2-yy1)*12);
//    return i;
//}

//int MainWindow::ComparingDatesmm(QDate dataone, QDate datatwo) {
//    QDate QTdataone = dataone;
//    QDate QTdatatwo = datatwo;
//    return QTdataone > QTdatatwo ? QTdataone == QTdatatwo ? 0 : 1 : 2;
//}

//int MainWindow::ComparingDatesdd(const char*dataone, const char*datatwo) {
//    int dd1, dd2, mm1, mm2, yy1, yy2;
//    int i;
//    sscanf(dataone,"%d.%d.%d", &dd1, &mm1, &yy1);
//    sscanf(datatwo,"%d.%d.%d", &dd2, &mm2, &yy2);
//    if (yy1 > yy2) i = 1;
//    else
//        if (yy1 < yy2) i = 2;
//        else // когда года равны
//            if (mm1 > mm2) i = 1;
//            else
//                if (mm1 < mm2) i = 2;
//                else // когда равны года и месяцы
//                    if (dd1 > dd2) i = 1;
//                    else
//                        if (dd1 < dd2) i = 2;
//                        else // даты совпадают
//                            i = 0;
//    if (i == 1) {
//        qDebug()<<"Первая дата более поздняя";
//    } else if (i == 2) {
//        qDebug()<<"Вторая дата более поздняя";
//    } else {
//        qDebug()<<"Даты совпадают";
//    }
//    return i;
//}

QDate MainWindow::ValidationDateVerification(QString Data, QString WhatIsDate,bool msgBoxBool)
{
    QDate DateValue;

    QString DateFormatString;

    if (DateFormatList.isEmpty()) {
        qDebug()<<"DateFormatList.isEmpty()";
        DateFormatList << "MM.yyyy"<<"yyyy.MM"<<"dd.MM.yyyy"<<"yyyy.MM.dd"<<"MM.yy"<<"dd:MM:yy"<<"yyyy:MM"<<"MM:yyyy"<<"dd:MM:yyyy"<<"yyyy:MM:dd"<<"MM/yyyy"<<"yyyy/MM"<<"yyyy/MM/dd"<<"dd/MM/yyyy"<<"yyyy\\MM"<<"MM\\yyyy"<<"dd\\MM\\yyyy"<<"yyyy\\MM\\dd";
    }

    for (int i = 0; i < DateFormatList.size(); ++i) {
        if(i%2 == 0) {
            DateFormatString+=QString("'%1'/n").arg(DateFormatList.at(i));
        } else {
            DateFormatString+=QString("'%1'").arg(DateFormatList.at(i));
        }
        DateValue = QDate::fromString(Data, DateFormatList.at(i));
        if (DateValue.isValid()) {
            //            qDebug()<<"___________________________________________________________";
            //            qDebug()<<"DateValue"<<DateValue.isValid()<<DateFormatList.at(i)<<DateValue.toString(DateFormatList.at(i))<<"WhatIsDate"<<WhatIsDate;
            //            qDebug()<<"___________________________________________________________";
            return DateValue;
        } else {
            if (IgnoreString!=true && msgBoxBool==true && i==DateFormatList.size()-1 && !DateValue.isValid()) {
                msgBox.setText(QString("Дата должна быть в формате:\n"
                                       "%1\n"
                                       "Дата: %2, не соответствуют этим форматам.").arg(DateFormatString).arg(WhatIsDate));
                msgBox.exec();
            }
            if (i==DateFormatList.size()-1 && !DateValue.isValid()) {
                return DateValue;
            }
        }
    }
}

void MainWindow::on_pushButton_4_clicked()
{
    if (ui->tableView->model()) {
        if(ui->tableView->model()->rowCount()!=0){
            theLoader.makeRequest("https://ru.wikipedia.org/w/api.php?action=query&prop=revisions&titles=Ключевая%20ставка%20в%20России&rvprop=content&rvsection=1&format=xml");
        }
    } else {
        msgBox.setText(QString("Определите в начале задолженности, нажав на кнопку 'Рассчитать'."));
        msgBox.exec();
        return;
    }

}

void MainWindow::dataUploader(QString **arrKeyRate, int row_arr_KeyRate)
{
    //    for(int j=0; j<row_arr_KeyRate;j++) {
    //        qDebug()<<"MainWindow arrKeyRate"<<arrKeyRate[j][0]<<arrKeyRate[j][1]<<arrKeyRate[j][2];
    //    }
    //    for(int j=0; j<=end_Debit_row;j++) {
    //        qDebug()<<"MainWindow arrDuty"<<arrDuty[j][0]<<arrDuty[j][1]<<arrDuty[j][2];
    //    }
    model->clear();
    ui->tableView->setModel(model);
    count_row_Duty = 0;

    QDate date_from_Duty;
    QDate date_to_Duty;
    QDate date_from_KeyRate;
    QDate date_to_KeyRate;

    QStringList horizontalHeader;
    horizontalHeader.append("№");
    horizontalHeader.append("Плата,\nначисленная\nза месяц");
    horizontalHeader.append("Крайний срок\nдля внесения\nплаты");
    horizontalHeader.append("Начало\nпериода\nпросрочки");
    horizontalHeader.append("с");
    horizontalHeader.append("по");
    horizontalHeader.append("Процентная\nставка");
    horizontalHeader.append("Коэффициент");
    horizontalHeader.append("Количество\nдней\nпросрочки");
    horizontalHeader.append("Неустойка,\nподлежащая\nуплате");

    model->setHorizontalHeaderLabels(horizontalHeader);
    ui->progressBar->show();
    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(end_Debit_row);
    sumDebt = 0;
    sumDebtAll = 0;
    for (int i=0; i<=end_Debit_row;i++) {

        if (arrDuty[i][0].isEmpty()) {
            triggerDuty = false;
        } else {
            date_from_Duty = MainWindow::ValidationDateVerification(arrDuty[i][0],"долг",true);
            if (!date_from_Duty.isValid()) {return;}
            triggerDuty = true;
        }
        QDate date_from_Duty_EarlyDelay = date_from_Duty;
        date_from_Duty_EarlyDelay = date_from_Duty_EarlyDelay.addDays(DeadlinePeriodDays+EarlyDelayPeriodDays).addMonths(DeadlinePeriodMonth+EarlyDelayPeriodMonth);

        if (arrDuty[i][2].isEmpty()) {
            triggerDuty = false;
        } else {
            date_to_Duty = MainWindow::ValidationDateVerification(arrDuty[i][2],"долг",true);
            if (!date_to_Duty.isValid()) {return;}
            triggerDuty = true;
        }

        if (triggerDuty==true) {
            count_row_Duty++;
            countDay = 0;
            Coefficient_row = 0;
            triggerDate=false;
            //            begin_row = 0;
            //            count_row_KeyRate = 0;

            for(int j=0;j<row_arr_KeyRate;++j) {

                date_from_KeyRate = MainWindow::ValidationDateVerification(arrKeyRate[j][0],"Ключевая ставка",true);
                if (!date_from_KeyRate.isValid()) {return;}

                if (arrKeyRate[j][1]=="н.в.") {
                    date_to_KeyRate = count_date_end;
                } else {
                    date_to_KeyRate = MainWindow::ValidationDateVerification(arrKeyRate[j][1],"Ключевая ставка",true);
                    if (!date_to_KeyRate.isValid()) {return;}
                }

                if (date_to_Duty<=date_to_KeyRate) {
                    date_to_KeyRate=date_to_Duty;
                }

                if (date_to_Duty<=date_from_KeyRate) {
                    item = new QStandardItem("Итоговый размер пени:");
                    model->setItem(count_row_Duty, 6, item);
                    item = new QStandardItem(QString::number(sumDebt, 'f', 2).replace(".",","));
                    model->setItem(count_row_Duty, 9, item);
                    ui->tableView->setModel(model);
                    ui->tableView->setSpan(count_row_Duty,6,1,3);
                    ui->tableView->setSpan(begin_row,1,count_row_KeyRate,1);
                    ui->tableView->setSpan(begin_row,2,count_row_KeyRate,1);
                    ui->tableView->setSpan(begin_row,3,count_row_KeyRate,1);
                    sumDebt =0;
                    count_row_KeyRate=0;
                    count_row_Duty++;
                    break;
                }

                if (date_from_Duty==date_to_Duty) {

                } else if (date_to_Duty.addMonths(-1).month()==date_from_Duty.month()  && date_to_Duty.year() == date_from_Duty.year()) {

                } else if (date_to_Duty.addMonths(-2).month()==date_from_Duty.month()  && date_to_Duty.year() == date_from_Duty.year()) {

                } else if (date_from_KeyRate<=date_from_Duty_EarlyDelay && date_to_KeyRate>=date_from_Duty_EarlyDelay && triggerDate==false) {

                    QDate d1(date_from_Duty_EarlyDelay.year(), date_from_Duty_EarlyDelay.month(), date_from_Duty_EarlyDelay.day());
                    QDate d2(date_to_KeyRate.year(), date_to_KeyRate.month(), date_to_KeyRate.day());
                    i = MainWindow::TableConstruction(d1,d2,arrKeyRate[j][2],i,true,false);

                } else if (date_from_KeyRate<=date_to_Duty && date_to_KeyRate>=date_to_Duty) {

                    QDate d1(date_from_KeyRate.year(), date_from_KeyRate.month(), date_from_KeyRate.day());
                    QDate d2(date_to_Duty.year(), date_to_Duty.month(), date_to_Duty.day());
                    i = MainWindow::TableConstruction(d1,d2,arrKeyRate[j][2],i,false,true);
                    //break;

                } else if (triggerDate==true) {

                    QDate d1(date_from_KeyRate.year(), date_from_KeyRate.month(), date_from_KeyRate.day());
                    QDate d2(date_to_KeyRate.year(), date_to_KeyRate.month(), date_to_KeyRate.day());
                    i = MainWindow::TableConstruction(d1,d2,arrKeyRate[j][2],i);

                }
                //                if (j==row_arr_KeyRate-1) {
                //                }
            }
        }
        if (i==end_Debit_row) {
            count_row_Duty++;
            item = new QStandardItem("Размер неустйоки за весь период:");
            model->setItem(count_row_Duty, 6, item);
            item = new QStandardItem(QString::number(sumDebtAll, 'f', 2).replace(".",","));
            model->setItem(count_row_Duty, 9, item);
            ui->tableView->setModel(model);
            ui->tableView->setSpan(count_row_Duty,6,1,3);
        }
        ui->progressBar->setValue(i);
    }

    ui->tableView->setModel(model);
    for (int c = 0; c < ui->tableView->horizontalHeader()->count(); ++c)
    {
        ui->tableView->horizontalHeader()->setSectionResizeMode(c, QHeaderView::Stretch);
    }
    ui->progressBar->hide();
}

int MainWindow::TableConstruction (QDate d1, QDate d2, QString arrKeyRate_value, int i, bool triggerBegin, bool triggerEnd)
{

    countDay += (int)d1.daysTo(d2);

    if (arrCoefficient[Coefficient_row][2]!="н.в." && countDay>arrCoefficient[Coefficient_row][2].toInt()) {

        QDate d3 = d1;

        while (arrCoefficient[Coefficient_row][2]!="н.в." && countDay>arrCoefficient[Coefficient_row][2].toInt()) {

            int countDayOld = abs(countDay - (int)d3.daysTo(d2));
            int countDayDifference = abs(arrCoefficient[Coefficient_row][2].toInt() - countDayOld);

            if (countDayDifference==0) {
                countDay = countDay-arrCoefficient[Coefficient_row][2].toInt();
                Coefficient_row++;
                break;
            } else {
                item = new QStandardItem(QString::number(count_row_KeyRate));
                model->setItem(count_row_Duty, 0, item);

                if (d3 == d1 && triggerBegin == true) {
                    item = new QStandardItem(arrDuty[i][1]);
                    model->setItem(count_row_Duty, 1, item);
                    item = new QStandardItem(d1.addDays(-EarlyDelayPeriodDays).addMonths(-EarlyDelayPeriodMonth).toString("dd.MM.yyyy"));
                    model->setItem(count_row_Duty, 2, item);
                    item = new QStandardItem(d1.toString("dd.MM.yyyy"));
                    model->setItem(count_row_Duty, 3, item);

                    begin_row = count_row_Duty;
                }

                item = new QStandardItem(d3.toString("dd.MM.yyyy"));
                model->setItem(count_row_Duty, 4, item);
                item = new QStandardItem(d3.addDays(countDayDifference).toString("dd.MM.yyyy"));
                model->setItem(count_row_Duty, 5, item);
                item = new QStandardItem(arrKeyRate_value.replace(".",","));
                model->setItem(count_row_Duty, 6, item);
                Coefficient_value = arrCoefficient[Coefficient_row][1];
                item = new QStandardItem(arrCoefficient[Coefficient_row][1].replace(".",","));
                model->setItem(count_row_Duty, 7, item);
                daysToDate = (int)d3.daysTo(d3.addDays(countDayDifference));
                item = new QStandardItem(QString::number(daysToDate));
                model->setItem(count_row_Duty, 8, item);
                Debt = ((double)daysToDate*arrDuty[i][1].replace(",",".").toDouble()*arrKeyRate_value.replace(",",".").toDouble()/Coefficient_value.replace("1/","").toDouble())/100;
                item = new QStandardItem(QString::number(Debt, 'f', 2).replace(".",","));
                model->setItem(count_row_Duty, 9, item);

                sumDebt +=Debt;
                sumDebtAll +=Debt;

                count_row_KeyRate++;
                count_row_Duty++;

                countDay = abs(countDay-arrCoefficient[Coefficient_row][2].toInt());
                Coefficient_row++;

                d3 = d3.addDays(countDayDifference);
            }
        }

        item = new QStandardItem(QString::number(count_row_KeyRate));
        model->setItem(count_row_Duty, 0, item);
        item = new QStandardItem(d3.toString("dd.MM.yyyy"));
        model->setItem(count_row_Duty, 4, item);
        item = new QStandardItem(d2.toString("dd.MM.yyyy"));
        model->setItem(count_row_Duty, 5, item);
        item = new QStandardItem(arrKeyRate_value.replace(".",","));
        model->setItem(count_row_Duty, 6, item);
        Coefficient_value = arrCoefficient[Coefficient_row][1];
        item = new QStandardItem(arrCoefficient[Coefficient_row][1].replace(".",","));
        model->setItem(count_row_Duty, 7, item);
        daysToDate = (int)d3.daysTo(d2);
        item = new QStandardItem(QString::number(daysToDate));
        model->setItem(count_row_Duty, 8, item);
        Debt = ((double)daysToDate*arrDuty[i][1].replace(",",".").toDouble()*arrKeyRate_value.replace(",",".").toDouble()/Coefficient_value.replace("1/","").toDouble())/100;
        item = new QStandardItem(QString::number(Debt, 'f', 2).replace(".",","));
        model->setItem(count_row_Duty, 9, item);

        sumDebt +=Debt;
        sumDebtAll +=Debt;

        count_row_KeyRate++;
        count_row_Duty++;

    } else {
        item = new QStandardItem(QString::number(count_row_KeyRate));
        model->setItem(count_row_Duty, 0, item);

        if (triggerBegin == true) {
            item = new QStandardItem(arrDuty[i][1]);
            model->setItem(count_row_Duty, 1, item);
            item = new QStandardItem(d1.addDays(-EarlyDelayPeriodDays).addMonths(-EarlyDelayPeriodMonth).toString("dd.MM.yyyy"));
            model->setItem(count_row_Duty, 2, item);
            item = new QStandardItem(d1.toString("dd.MM.yyyy"));
            model->setItem(count_row_Duty, 3, item);
            begin_row = count_row_Duty;
        }

        item = new QStandardItem(d1.toString("dd.MM.yyyy"));
        model->setItem(count_row_Duty, 4, item);
        item = new QStandardItem(d2.toString("dd.MM.yyyy"));
        model->setItem(count_row_Duty, 5, item);
        item = new QStandardItem(arrKeyRate_value.replace(".",","));
        model->setItem(count_row_Duty, 6, item);
        Coefficient_value = arrCoefficient[Coefficient_row][1];
        item = new QStandardItem(arrCoefficient[Coefficient_row][1].replace(".",","));
        model->setItem(count_row_Duty, 7, item);
        daysToDate = (int)d1.daysTo(d2);
        item = new QStandardItem(QString::number(daysToDate));
        model->setItem(count_row_Duty, 8, item);
        Debt = ((double)daysToDate*arrDuty[i][1].replace(",",".").toDouble()*arrKeyRate_value.replace(",",".").toDouble()/Coefficient_value.replace("1/","").toDouble())/100;
        item = new QStandardItem(QString::number(Debt, 'f', 2).replace(".",","));
        model->setItem(count_row_Duty, 9, item);

        sumDebt +=Debt;
        sumDebtAll +=Debt;

        count_row_KeyRate++;
        count_row_Duty++;

    }

    if (triggerEnd == true) {
        triggerDate = false;
    } else if (triggerBegin == true) {
        triggerDate = true;
    }
    return i;
}

void MainWindow::saveAsCSV(QString filename)
{
    QFile f(filename);

    if( f.open( QIODevice::WriteOnly ) )
    {
        QTextStream ts( &f );
        QStringList strList;

        strList << "\" \"";
        for( int c = 0; c < ui->tableView->horizontalHeader()->count(); ++c )
            strList << "\""+ui->tableView->model()->headerData(c, Qt::Horizontal).toString()+"\"";
        ts << strList.join( ";" )+"\n";

        for( int r = 0; r < ui->tableView->verticalHeader()->count(); ++r )
        {
            strList.clear();
            strList << "\""+ui->tableView->model()->headerData(r, Qt::Vertical).toString()+"\"";
            for( int c = 0; c < ui->tableView->horizontalHeader()->count(); ++c )
            {
                strList << "\""+ui->tableView->model()->data(ui->tableView->model()->index(r, c), Qt::DisplayRole).toString()+"\"";
            }
            ts << strList.join( ";" )+"\n";
        }
        f.close();
    }
}

void MainWindow::on_pushButton_export_to_CSV_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Расчет пеней"), "",
                                                    tr("Файлы таблиц (*.csv);; All Files (*)"));

    if (!fileName.isEmpty()) {
        MainWindow::saveAsCSV(fileName);
    } else {
        msgBox.setText(QString("Вы не сохранили!\nНазовите и выберите куда сохранить файл."));
        msgBox.exec();
    }
}

void MainWindow::on_pushButton_export_to_xlsx_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Расчет пеней"), "",
                                                    tr("Файлы таблиц (*.xlsx);; All Files (*)"));
    fileName.replace("/","\\");
    try
    {
        ExcelExportHelper helper;
        int row_count = ui->tableView->model()->rowCount();
        int column_count = ui->tableView->model()->columnCount();
        qDebug()<<"==========================================";
        ui->progressBar->show();
        ui->progressBar->setMinimum(1);
        ui->progressBar->setMaximum(row_count);
        for (int row=1; row<=row_count;row++) {
            for (int column=1; column<=column_count;column++) {
                if (!ui->tableView->model()->data(ui->tableView->model()->index(row-1,column-1)).isNull()) {
                    qDebug()<<row<<column<<ui->tableView->model()->data(ui->tableView->model()->index(row-1,column-1)).toString();
                    if( !(ui->tableView->model()) )  continue;
                    qDebug() << Q_FUNC_INFO << "row" << row << "column" << column;
                    qDebug() << Q_FUNC_INFO <<"type"<< ui->tableView->model()->data(ui->tableView->model()->index(row-1,column-1)).type();
                    QString value = ui->tableView->model()->data(ui->tableView->model()->index(row-1,column-1)).toString();
                    bool boolDate = MainWindow::DetermineNumberORDate(value);
                    if (boolDate==true) {
                        qDebug() << Q_FUNC_INFO <<boolDate << "value" << value;
                        helper.SetCellValue(row,column,value.replace(".","-"));
                    } else {
                        qDebug() << Q_FUNC_INFO <<boolDate << "value" << value;
                        helper.SetCellValue(row,column,value.replace(".",","));
                    }
                }
            }
            ui->progressBar->setValue(row);
        }
        ui->progressBar->hide();
        qDebug()<<"==========================================";
        helper.SaveAs(fileName);
    }
    catch (const std::exception& e)
    {
        QMessageBox::critical(this, "Ошибка", e.what());
    }
}

bool MainWindow::DetermineNumberORDate(QString data)
{
    QRegularExpression re("^(([0-9]{1,4}([.|:|-|_| |/|\\\\]{1,4})){1,5}[0-9]{1,4})$");
    QRegularExpressionMatch match = re.match(data);
    bool hasMatch = match.hasMatch();
    return hasMatch;
}

void MainWindow::on_action_triggered()
{
    SettingsPennyDialog = new SettingsPenny(Settings);
    SettingsPennyDialog->setWindowFlags (SettingsPennyDialog->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    SettingsPennyDialog->setAttribute(Qt::WA_DeleteOnClose);
    SettingsPennyDialog->show();
    connect( SettingsPennyDialog, SIGNAL(destroyed(QObject*)), this, SLOT(getSettings()));
}

void MainWindow::on_action_2_triggered()
{
    SettingsDateFormatDialog = new SettingsDateFormat(Settings);
    SettingsDateFormatDialog->setWindowFlags (SettingsDateFormatDialog->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    SettingsDateFormatDialog->setAttribute(Qt::WA_DeleteOnClose);
    SettingsDateFormatDialog->show();
    connect( SettingsDateFormatDialog, SIGNAL(destroyed(QObject*)), this, SLOT(getSettings()));
}

void MainWindow::on_aboutprogram_triggered()
{
    AboutProgramDialog *APDialog = new AboutProgramDialog();
    APDialog->setWindowFlags (APDialog->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    APDialog->setAttribute(Qt::WA_DeleteOnClose);
    APDialog->show();
}
