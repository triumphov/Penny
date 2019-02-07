#include "settingsbase.h"

SettingsBase::SettingsBase(QDialog *parent) :
QDialog(parent)
{

}

SettingsBase::~SettingsBase()
{

}

void SettingsBase::TableBuilding(QString nameSetting) {

    QStringList horizontalHeader;
    if (nameSetting=="Penny") {
        horizontalHeader.append("Значение коэффициента");
        //horizontalHeader.append("Временной период");
        horizontalHeader.append("Дней:");
        horizontalHeader.append("Месяцев:");
        horizontalHeader.append("Кнопка удаления");
    } else if (nameSetting=="DateFormat") {
        horizontalHeader.append("Форматы даты");
        horizontalHeader.append("Кнопка удаления");
    }

    model->setHorizontalHeaderLabels(horizontalHeader);

    if (nameSetting=="Penny") {
        //Первый ряд
        item = new QStandardItem(QString("1/300"));
        model->setItem(0, 0, item);
        item = new QStandardItem(QString("90"));
        model->setItem(0, 1, item);
        item = new QStandardItem(QString("0"));
        model->setItem(0, 2, item);
        item = new QStandardItem(QString(""));
        model->setItem(0, 3, item);
        //Второй ряд
        item = new QStandardItem(QString("1/130"));
        model->setItem(1, 0, item);
        item = new QStandardItem(QString("н.в."));
        model->setItem(1, 1, item);
        item = new QStandardItem(QString("н.в."));
        model->setItem(1, 2, item);
        item = new QStandardItem(QString(""));
        model->setItem(1, 3, item);
    } else if (nameSetting=="DateFormat") {
        QStringList DateFormatList;
        DateFormatList << "MM.yyyy"<<"yyyy.MM"<<"dd.MM.yyyy"<<"yyyy.MM.dd"<<"MM.yy"<<"dd:MM:yy"<<"yyyy:MM"<<"MM:yyyy"<<"dd:MM:yyyy"<<"yyyy:MM:dd"<<"MM/yyyy"<<"yyyy/MM"<<"yyyy/MM/dd"<<"dd/MM/yyyy"<<"yyyy\\MM"<<"MM\\yyyy"<<"dd\\MM\\yyyy"<<"yyyy\\MM\\dd";
        for (int i=0;i<DateFormatList.size();i++) {
            item = new QStandardItem(DateFormatList.at(i));
            model->setItem(i, 0, item);
            item = new QStandardItem("");
            model->setItem(i, 1, item);
        }
    }

    settingsTableView->setModel(model);



    if (nameSetting=="Penny") {
        ButtonDeleteColumnDelegate *ButtonDelete = new ButtonDeleteColumnDelegate(settingsTableView);
        settingsTableView->setItemDelegateForColumn(3, ButtonDelete);
    } else if (nameSetting=="DateFormat") {
        ButtonDeleteColumnDelegate *ButtonDelete = new ButtonDeleteColumnDelegate(settingsTableView);
        settingsTableView->setItemDelegateForColumn(1, ButtonDelete);
    }

    for (int c = 0; c < settingsTableView->horizontalHeader()->count(); ++c)
    {
        settingsTableView->horizontalHeader()->setSectionResizeMode(c, QHeaderView::Stretch);
    }
}

QStringList SettingsBase::JSONSettingsPath(const QString &path) {
    QDir dir(path);
    QStringList::Iterator it;
    QStringList JSONSettingsPathList;
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
            SettingsBase::JSONSettingsPath(path + "/" + *it);
        ++it;
    }
    return JSONSettingsPathList;
}
