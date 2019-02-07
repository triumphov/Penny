#ifndef SETTINGSBASE_H
#define SETTINGSBASE_H

#include <QDialog>
#include <QFileInfo>

#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QDir>

#include <QTableView>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QStandardItemModel>

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#include "buttondeletecolumndelegate.h"

class SettingsBase : public QDialog
{
    Q_OBJECT
public:
        explicit SettingsBase(QDialog *parent = nullptr);
        virtual ~SettingsBase();
        virtual void ApplySettings(QFileInfo info) = 0;
        virtual QStringList JSONSettingsPath(const QString &path);
        QMessageBox msgBox;
        QTableView *settingsTableView = new QTableView(this);
        QStandardItemModel *model = new QStandardItemModel;
        QStandardItem *item;

public slots:
    virtual void DefaultValue() = 0;
    virtual void TableBuilding(QString nameSetting);
    virtual void AddButton() = 0;
    virtual void SaveSettings(bool DialogClose = true) = 0;

};

#endif // SETTINGSBASE_H
