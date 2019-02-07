#include "settingsdateformat.h"
#include <QRegExp>
#include <QRegExpValidator>

SettingsDateFormat::SettingsDateFormat(SettingsBase *SettB):Settings(SettB)
{
    SettingsDateFormat::setWindowTitle(tr("Настройка формата даты"));
    //SettingsDateFormat::setWindowIcon(QIcon(QDir::current().absolutePath()+"/icon/calendar.ico"));
    SettingsDateFormat::setWindowIcon(QIcon(":/icon/calendar.png"));
    SettingsDateFormat::setStyleSheet("padding: 10px;");
    QGridLayout *layout = new QGridLayout;
    layout->setSpacing(10);
    layout->setMargin(15);
    settingsTableView->setStyleSheet("padding: 0px;");
    layout->addWidget(settingsTableView,0,0,6,1);
    LineEdit = new QLineEdit(this);
    QRegExp DateRegex("^(([d|M|y|h|H|m|s]{1,4}([.|,|:|;|-|_|+| |/|\\\\]{1,4})){1,5}[d|M|y|h|H|m|s]{1,4})$");
    QRegExpValidator *validator = new QRegExpValidator(DateRegex, this);
    LineEdit->setValidator(validator);
    dateFormatAddButton = new QPushButton("Добавить формат даты",this);
    layout->addWidget(LineEdit,0,1);
    layout->addWidget(dateFormatAddButton,1,1);
    QObject::connect(dateFormatAddButton,SIGNAL(clicked(bool)),this,SLOT(AddButton()));
    LabelFormatHead->setStyleSheet("background: lightgray; padding: 5px; font-size: 15px;");
    layout->addWidget(LabelFormatHead,2,1);
    LabelFormatHeadFormatDate->setStyleSheet("background: lightgray; padding: 5px; font-size: 15px;");
    layout->addWidget(LabelFormatHeadFormatDate,3,1);
    LabelDateTextFormat->setStyleSheet("border-top: 3px double; border-bottom: 3px double;");
    layout->addWidget(LabelDateTextFormat,4,1);
    LabelTimeTextFormat->setStyleSheet("border-top: 3px double; border-bottom: 3px double;");
    layout->addWidget(LabelTimeTextFormat,5,1);
    QPushButton *DefaultValueButton = new QPushButton("Востановить значения по умолчанию",this);
    QObject::connect(DefaultValueButton,SIGNAL(clicked(bool)),this,SLOT(DefaultValue()));
    layout->addWidget(DefaultValueButton,6,0,1,2);
    QPushButton *SaveButton = new QPushButton("Сохранить");
    QObject::connect(SaveButton,SIGNAL(clicked(bool)),this,SLOT(SaveSettings()));
    layout->addWidget(SaveButton,7,0,1,2);

    QStringList Setting = JSONSettingsPath(QDir::currentPath());
    bool JSONBool = false;
    for (int x=0;x<Setting.size();x++) {
        QFileInfo info(Setting.at(x));
        if (info.fileName()=="formatDate.json") {
            SettingsDateFormat::ApplySettings(info);
            JSONBool = true;
            break;
        }
    }

    if (JSONBool != true) {
        msgBox.setText(QString("Файл форматов дат не найден! Будут применены настройки по умолчанию."));
        msgBox.exec();
        DefaultValue();
    }

    this->setLayout(layout);
}

void SettingsDateFormat::ApplySettings(QFileInfo info) {
    if (info.fileName()=="formatDate.json") {
        QString SettingsPath = info.absoluteFilePath();
        QFile loadFile(SettingsPath);
        if (!loadFile.open(QIODevice::ReadOnly)) {
            QMessageBox::critical(this, tr("Ошибка"), tr("Нельзя открыть файл!"));
            return;
        } else {
            QByteArray SettingsData = loadFile.readAll();
            QJsonDocument loadDoc(QJsonDocument::fromJson(SettingsData));

            QJsonArray jsonArray = loadDoc["Format"].toArray();
            model->clear();
            settingsTableView->setModel(model);
            QStringList horizontalHeader;
            horizontalHeader.append("Форматы даты");
            horizontalHeader.append("Кнопка удаления");
            model->setHorizontalHeaderLabels(horizontalHeader);
            int b = 0;
            foreach (const QJsonValue & value, jsonArray) {
                QJsonObject obj = value.toObject();
                item = new QStandardItem(obj["dateFormat"].toString());
                model->setItem(b, 0, item);
                item = new QStandardItem("");
                model->setItem(b, 1, item);
                b++;
            }
            settingsTableView->setModel(model);
            ButtonDeleteColumnDelegate *ButtonDelete = new ButtonDeleteColumnDelegate(settingsTableView);
            settingsTableView->setItemDelegateForColumn(1, ButtonDelete);
            for (int c = 0; c < settingsTableView->horizontalHeader()->count(); ++c)
            {
                settingsTableView->horizontalHeader()->setSectionResizeMode(c, QHeaderView::Stretch);
            }
        }
    }
}

void SettingsDateFormat::DefaultValue()
{
    TableBuilding("DateFormat");
    SettingsDateFormat::SaveSettings(false);
}

void SettingsDateFormat::AddButton()
{
    int row=model->rowCount();
    item = new QStandardItem(LineEdit->text());
    model->setItem(row, 0, item);
    item = new QStandardItem("");
    model->setItem(row, 1, item);
    settingsTableView->setModel(model);
}

void SettingsDateFormat::SaveSettings(bool DialogClose) {
    QString PathToSettingsFiles = "formatDate.json";
    QStringList JSONSettingsPathList = JSONSettingsPath(QDir::currentPath());
    for (int i = 0; i < JSONSettingsPathList.size(); ++i) {
        QFileInfo info(JSONSettingsPathList.at(i));
        if (info.fileName()=="formatDate.json") {
            QFile(info.absoluteFilePath()).remove();
            msgBox.setText("Старые настройки удалены: "+info.fileName()+".");
            msgBox.exec();
        }
    }
    QFileInfo fi(QDir::currentPath(), PathToSettingsFiles);
    QFile JsonFile(fi.absoluteFilePath());
    JsonFile.open(QFile::WriteOnly);
    QJsonDocument JsonDoc;
    QJsonArray dateFormat_array;
    QString dateFormat_str("dateFormat");
    for (int x=0; x<model->rowCount(); x++) {
        QJsonObject item_data;
        item_data.insert(dateFormat_str, QJsonValue(settingsTableView->model()->data(settingsTableView->model()->index(x,0)).toString()));
        dateFormat_array.push_back(QJsonValue(item_data));
    }
    QJsonObject final_object;
    final_object.insert(QString("Format"), QJsonValue(dateFormat_array));
    JsonDoc.setObject(final_object);
    JsonFile.write(JsonDoc.toJson());
    msgBox.setText("Настройки сохранены.");
    msgBox.exec();
    if (DialogClose==true) {
        this->close();
    }
    return;
}
