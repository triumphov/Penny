#include "settingspenny.h"

SettingsPenny::SettingsPenny(SettingsBase *SettB):Settings(SettB)
{
    SettingsPenny::setWindowTitle(tr("Настройки основных параметров расчёта пени"));
    SettingsPenny::setWindowIcon(QIcon(":/icon/settings.png"));
    //    SettingsPenny::setStyleSheet("background: lightgray; color: #674C33; padding: 5px;");
    //Settings::setStyleSheet( "background-image:url(:/icon/settings.png); background-repeat: no-repeat;");
    QGridLayout *layout = new QGridLayout;
    layout->setSpacing(10);
    layout->setMargin(15);
    labelDebitException = new QLabel("Установить минимальную и максимальную сумму дебита в рублях, которая будет\nисключаться в расчётах:");
    labelRuble = new QLabel(tr("рубля(₽)"));
    labelSliderMinVal = new QLabel(this);
    labelSliderMaxVal = new QLabel(this);
    labelDeadlineDays = new QLabel("Период до крайнего срока для внесения платы:");
    labelEarlyDelayDays = new QLabel("Начало периода просрочки:");
    QLabel *help = new QLabel("Значения\nсуммируются.");

    SBDeadlineDaysSpinBox = new QSpinBox(this);
    SBDeadlineDaysSpinBox->setMinimum(0);
    SBDeadlineDaysSpinBox->setMaximum(31);
    QLabel *DaysLabel = new QLabel("дней");
    SBDeadlineMonthSpinBox = new QSpinBox(this);
    QLabel *MonthLabel = new QLabel("месяцев");

    SBEarlyDelayDaysSpinBox = new QSpinBox(this);
    SBEarlyDelayDaysSpinBox->setMinimum(0);
    SBEarlyDelayDaysSpinBox->setMaximum(31);
    SBEarlyDelayMonthSpinBox = new QSpinBox(this);

    SBIndex = new QSpinBox(this);
    SBDays = new QSpinBox(this);
    SBDays->setMinimum(0);
    SBDays->setMaximum(366);
    SBDays->setValue(90);
    SBMonth = new QSpinBox(this);

    QLabel *LabelValueIndex = new QLabel("Значение коэффициента:");
    LabelIndex = new QLabel(this);

    LabelDays = new QLabel("количество дней");
    LabelMonth = new QLabel("количество месяцев");
    LabelCoefficient = new QLabel("действия\nкоэффициента.");

    layout->addWidget(labelDebitException,0,0);
    layout->addWidget(labelSliderMinVal,0,1);
    layout->addWidget(Slider,0,2);
    layout->addWidget(labelSliderMaxVal,0,3);
    layout->addWidget(labelRuble,0,4,1,2);

    layout->addWidget(labelDeadlineDays,1,0);
    layout->addWidget(SBDeadlineDaysSpinBox,1,1);
    layout->addWidget(DaysLabel,1,2,2,1);
    layout->addWidget(SBDeadlineMonthSpinBox,1,3);
    layout->addWidget(MonthLabel,1,4,2,1);

    layout->addWidget(labelEarlyDelayDays,2,0);
    layout->addWidget(SBEarlyDelayDaysSpinBox,2,1);
    layout->addWidget(SBEarlyDelayMonthSpinBox,2,3);

    layout->addWidget(help,1,5,2,1);
    settingsTableView->setStyleSheet("padding: 0px;");
    settingsTableView->setFixedWidth(480);
    settingsTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    TableBuilding("Penny");
    layout->addWidget(settingsTableView,3,0,4,1);
    SBIndex->setMinimum(-10000);
    SBIndex->setMaximum(10000);
    SBIndex->setValue(300);
    layout->addWidget(SBIndex,3,1);
    layout->addWidget(LabelValueIndex,3,2);
    LabelIndex->setText(QString("1/%1").arg(SBIndex->value()));
    layout->addWidget(LabelIndex,3,3);

    layout->addWidget(SBDays,4,1);
    layout->addWidget(LabelDays,4,2);
    SBMonth->setDisabled(true);
    layout->addWidget(SBMonth,4,3);
    layout->addWidget(LabelMonth,4,4);
    layout->addWidget(LabelCoefficient,4,5);

    layout->addWidget(NVCheckBox,5,2);
    QObject::connect(NVCheckBox,SIGNAL(toggled(bool)),this,SLOT(monitorCheckStatus()));
    layout->addWidget(indexAddButton,6,1,1,5);
    QObject::connect(indexAddButton,SIGNAL(clicked(bool)),this,SLOT(AddButton()));

    Slider->setOrientation(Qt::Horizontal);
    //Slider->setMinimumValue(-100000);
    Slider->setMinimum(-100000);
    Slider->setMaximum(100000);
    Slider->setStyleSheet("padding: 0px;");
    labelSliderMinVal->setText(QString::number(Slider->minimumValue()));
    labelSliderMaxVal->setText(QString::number(Slider->maximumValue()));

    QObject::connect(Slider,SIGNAL(minimumValueChanged(int)),this,SLOT(minimumValueChangedSlider(int)));
    QObject::connect(Slider,SIGNAL(maximumValueChanged(int)),this,SLOT(maximumValueChangedSlider(int)));
    QObject::connect(SBIndex,SIGNAL(valueChanged(int)),this,SLOT(SBIndexValueChanged(int)));

    QVBoxLayout *VBoxDebit = new QVBoxLayout;
    //    lessRadioDebit->setChecked(false);
    QObject::connect(lessRadioDebit,SIGNAL(clicked(bool)),this,SLOT(ClickkedStateDebit()));
    //    moreRadioDebit->setChecked(true);
    QObject::connect(moreRadioDebit,SIGNAL(clicked(bool)),this,SLOT(ClickkedStateDebit()));
    VBoxDebit->addWidget(lessRadioDebit);
    VBoxDebit->addWidget(moreRadioDebit);
    groupBoxDebit->setLayout(VBoxDebit);
    layout->addWidget(groupBoxDebit,7,0,1,6);

    checkboxIgnoreString = new QCheckBox("Игнорировать строки в датах.", this);
    checkboxConsiderPenny = new QCheckBox("Учитывать пени.", this);

    QGroupBox *documentsPennyGB = new QGroupBox(tr("Документы с пени"));
    QVBoxLayout *documentsPennyVbox = new QVBoxLayout;
    documentsPennyVbox->addWidget(checkboxIgnoreString);
    documentsPennyVbox->addWidget(checkboxConsiderPenny);
    documentsPennyGB->setLayout(documentsPennyVbox);
    layout->addWidget(documentsPennyGB,8,0);
    QObject::connect(DefaultValueButton,SIGNAL(clicked(bool)),this,SLOT(DefaultValue()));
    layout->addWidget(DefaultValueButton,9,0,1,6);
    QObject::connect(SaveButton,SIGNAL(clicked(bool)),this,SLOT(SaveSettings()));
    layout->addWidget(SaveButton,10,0,1,6);

    QStringList Setting = JSONSettingsPath(QDir::currentPath());
    bool JSONBool = false;
    for (int x=0;x<Setting.size();x++) {
        QFileInfo info(Setting.at(x));
        if (info.fileName()=="penny.json") {
            SettingsPenny::ApplySettings(info);
            JSONBool = true;
            break;
        }
    }

    if (JSONBool == false) {
        msgBox.setText(QString("Файл настроек не найден. Будут применены настройки по умолчанию."));
        msgBox.exec();
        DefaultValue();
    }

    this->setLayout(layout);
}

void SettingsPenny::ApplySettings(QFileInfo info)
{
    if (info.fileName()=="penny.json") {
        QString SettingsPath = info.absoluteFilePath();
        QFile loadFile(SettingsPath);
        if (!loadFile.open(QIODevice::ReadOnly)) {
            QMessageBox::critical(this, tr("Ошибка"), tr("Нельзя открыть файл!"));
            return;
        } else {
            //qDebug() << Q_FUNC_INFO << "Загрузить информацию о настройках программы.";
            QByteArray SettingsData = loadFile.readAll();
            QJsonDocument loadDoc(QJsonDocument::fromJson(SettingsData));

            Slider->setMinimumValue((loadDoc.object())["DebitMin"].toInt());
            Slider->setMaximumValue((loadDoc.object())["DebitMax"].toInt());

            SBDeadlineDaysSpinBox->setValue((loadDoc.object())["DeadlinePeriodDays"].toInt());
            SBDeadlineMonthSpinBox->setValue((loadDoc.object())["DeadlinePeriodMonth"].toInt());
            SBEarlyDelayDaysSpinBox->setValue((loadDoc.object())["EarlyDelayPeriodDays"].toInt());
            SBEarlyDelayMonthSpinBox->setValue((loadDoc.object())["EarlyDelayPeriodMonth"].toInt());

            QJsonArray jsonArray = loadDoc["coefficient"].toArray();
            model->clear();
            settingsTableView->setModel(model);
            QStringList horizontalHeader;
            horizontalHeader.append("Значение коэффициента");
            //horizontalHeader.append("Временной период");
            horizontalHeader.append("Дней:");
            horizontalHeader.append("Месяцев:");
            horizontalHeader.append("Кнопка удаления");
            model->setHorizontalHeaderLabels(horizontalHeader);
            foreach (const QJsonValue & value, jsonArray) {
                QJsonObject obj = value.toObject();
                item = new QStandardItem(obj["ValueIndex"].toString());
                model->setItem(obj["Id"].toInt(), 0, item);
                item = new QStandardItem(obj["PeriodDays"].toString());
                model->setItem(obj["Id"].toInt(), 1, item);
                item = new QStandardItem(obj["PeriodMonth"].toString());
                model->setItem(obj["Id"].toInt(), 2, item);
                item = new QStandardItem(QString(""));
                model->setItem(obj["Id"].toInt(), 3, item);
            }
            settingsTableView->setModel(model);
            for (int c = 0; c < settingsTableView->horizontalHeader()->count(); ++c)
            {
                settingsTableView->horizontalHeader()->setSectionResizeMode(c, QHeaderView::Stretch);
            }

            if ((loadDoc.object())["debt"].toBool() == true) {
                lessRadioDebit->setChecked(false);
                moreRadioDebit->setChecked(true);
            } else {
                lessRadioDebit->setChecked(true);
                moreRadioDebit->setChecked(false);
            }

            checkboxIgnoreString->setChecked((loadDoc.object())["IgnoreString"].toBool());
            checkboxConsiderPenny->setChecked((loadDoc.object())["ConsiderPenny"].toBool());

        }
    }
}

void SettingsPenny::DefaultValue()
{
    model->clear();
    settingsTableView->setModel(model);
    TableBuilding("Penny");
    Slider->setMinimumValue(-20000);
    Slider->setMaximumValue(20000);
    SBDeadlineDaysSpinBox->setValue(9);
    SBDeadlineMonthSpinBox->setValue(1);
    SBEarlyDelayDaysSpinBox->setValue(1);
    SBEarlyDelayMonthSpinBox->setValue(1);
    lessRadioDebit->setChecked(false);
    moreRadioDebit->setChecked(true);
    checkboxIgnoreString->setChecked(true);
    checkboxConsiderPenny->setChecked(true);
    SettingsPenny::SaveSettings(false);
}

void SettingsPenny::AddButton()
{
    QString Days;
    QString Month;
    bool NV = false;
    if (!NVCheckBox->isChecked()) {
        Days=SBDays->text();
        Month=SBMonth->text();
    } else {
        Days = "н.в.";
        Month = "н.в.";
    }
    for (int row=0;row<model->rowCount();row++) {
        if (settingsTableView->model()->data(settingsTableView->model()->index(row,1)).toString()=="н.в.") {
            settingsTableView->model()->removeRow(row);
            item = new QStandardItem(LabelIndex->text());
            model->setItem(row, 0, item);
            item = new QStandardItem(Days);
            model->setItem(row, 1, item);
            item = new QStandardItem(Month);
            model->setItem(row, 2, item);
            item = new QStandardItem(QString(""));
            model->setItem(row, 3, item);
            NV =true;
        } else {
            NV = false;
        }
    }
    if (NV==false) {
        int row=model->rowCount();
        item = new QStandardItem(LabelIndex->text());
        model->setItem(row, 0, item);
        item = new QStandardItem(Days);
        model->setItem(row, 1, item);
        item = new QStandardItem(Month);
        model->setItem(row, 2, item);
        item = new QStandardItem(QString(""));
        model->setItem(row, 3, item);
    }
    settingsTableView->setModel(model);
}

void SettingsPenny::SaveSettings(bool DialogClose)
{
    QString PathToSettingsFiles = "penny.json";
    QStringList JSONSettingsPathList = JSONSettingsPath(QDir::currentPath());
    for (int i = 0; i < JSONSettingsPathList.size(); ++i) {
        QFileInfo infoSettings(JSONSettingsPathList.at(i));
        if (infoSettings.fileName()=="penny.json") {
            QFile(JSONSettingsPathList.at(i)).remove();
            //        msgBox.setText("Старые настройки удалены: "+infoSettings.fileName()+".");
            //        msgBox.exec();
        }
    }
    QFileInfo fi(QDir::currentPath(), PathToSettingsFiles);
    QFile JsonFile(fi.absoluteFilePath());
    JsonFile.open(QFile::WriteOnly);
    QJsonDocument JsonDoc;
    QJsonArray coefficient_array;
    QString id_str("Id");
    QString vi_str("ValueIndex");
    QString periodDays_str("PeriodDays");
    QString periodMonth_str("PeriodMonth");

    for (int x=0; x<model->rowCount(); x++) {
        QJsonObject item_data;

        item_data.insert(id_str, QJsonValue(x));
        item_data.insert(vi_str, QJsonValue(settingsTableView->model()->data(settingsTableView->model()->index(x,0)).toString()));
        if (x==model->rowCount()-1) {
            item_data.insert(periodDays_str, QJsonValue("н.в."));
            item_data.insert(periodMonth_str, QJsonValue("н.в."));
        } else {
            item_data.insert(periodDays_str, QJsonValue(settingsTableView->model()->data(settingsTableView->model()->index(x,1)).toString()));
            item_data.insert(periodMonth_str, QJsonValue(settingsTableView->model()->data(settingsTableView->model()->index(x,2)).toString()));
        }

        coefficient_array.push_back(QJsonValue(item_data));
    }

    QJsonObject final_object;
    final_object.insert(QString("DebitMin"), QJsonValue(Slider->minimumValue()));
    final_object.insert(QString("DebitMax"), QJsonValue(Slider->maximumValue()));
    final_object.insert(QString("DeadlinePeriodDays"), QJsonValue(SBDeadlineDaysSpinBox->value()));
    final_object.insert(QString("DeadlinePeriodMonth"), QJsonValue(SBDeadlineMonthSpinBox->value()));
    final_object.insert(QString("EarlyDelayPeriodDays"), QJsonValue(SBEarlyDelayDaysSpinBox->value()));
    final_object.insert(QString("EarlyDelayPeriodMonth"), QJsonValue(SBEarlyDelayMonthSpinBox->value()));
    final_object.insert(QString("coefficient"), QJsonValue(coefficient_array));
    final_object.insert(QString("debt"), QJsonValue(boolValueDebit));
    final_object.insert(QString("IgnoreString"), QJsonValue(checkboxIgnoreString->isChecked()));
    final_object.insert(QString("ConsiderPenny"), QJsonValue(checkboxConsiderPenny->isChecked()));
    JsonDoc.setObject(final_object);
    JsonFile.write(JsonDoc.toJson());
    msgBox.setText("Настройки сохранены.");
    msgBox.exec();
    if (DialogClose==true) {
        this->close();
    }
    return;
}

// Функции только для SettingsPenny
void SettingsPenny::minimumValueChangedSlider(int min)
{
    labelSliderMinVal->setText(QString::number(min));
}

void SettingsPenny::maximumValueChangedSlider(int max)
{
    labelSliderMaxVal->setText(QString::number(max));
}

void SettingsPenny::SBIndexValueChanged (int value)
{
    LabelIndex->setText(QString("1/%1").arg(value));
}

void SettingsPenny::ClickkedStateDebit()
{
    if (lessRadioDebit->isChecked()) {
        boolValueDebit = false;
        msgBox.setText(QString("Подсчитать в строну уменьшения долга. %1").arg(boolValueDebit));
        msgBox.exec();
        return;
    } else if (moreRadioDebit->isChecked()) {
        boolValueDebit = true;
        msgBox.setText(QString("Подсчитать в строну увеличение долга. %1").arg(boolValueDebit));
        msgBox.exec();
        return;
    }
}

void SettingsPenny::monitorCheckStatus()
{
    if (!NVCheckBox->isChecked()) {
        SBDays->setVisible(true);
        SBDays->setEnabled(true);
        SBMonth->setVisible(true);
        SBMonth->setEnabled(true);
        SBMonth->setDisabled(true);
    } else {
        SBDays->setVisible(false);
        SBDays->setEnabled(false);
        SBMonth->setVisible(false);
        SBMonth->setEnabled(false);
        SBMonth->setDisabled(true);
    }
}
