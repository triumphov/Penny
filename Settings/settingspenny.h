#ifndef SETTINGSPENNY_H
#define SETTINGSPENNY_H

#include "settingsbase.h"
#include "ctkrangeslider.h"

#include <QSpinBox>
#include <QCheckBox>
#include <QRadioButton>
#include <QGroupBox>

class SettingsPenny : public SettingsBase
{
    Q_OBJECT
public:
    explicit SettingsPenny(SettingsBase *SettB);
    ~SettingsPenny() { delete Settings; }
    void ApplySettings(QFileInfo info);

private:
    SettingsBase* Settings;
    ctkRangeSlider *Slider = new ctkRangeSlider();
    QLabel *labelSliderMinVal = nullptr;
    QLabel *labelSliderMaxVal = nullptr;
    QLabel *labelDebitException = nullptr;
    QLabel *labelDeadlineDays = nullptr;
    QLabel *labelEarlyDelayDays = nullptr;

    QSpinBox *SBDeadlineDaysSpinBox = nullptr;
    QSpinBox *SBDeadlineMonthSpinBox = nullptr;
    QSpinBox *SBEarlyDelayDaysSpinBox = nullptr;
    QSpinBox *SBEarlyDelayMonthSpinBox = nullptr;

    QLabel *labelRuble = nullptr;
    QSpinBox *SBIndex = nullptr;
    QSpinBox *SBDays = nullptr;
    QSpinBox *SBMonth = nullptr;
    QLabel *LabelIndex = nullptr;
    QLabel *LabelDays = nullptr;
    QLabel *LabelMonth = nullptr;
    QLabel *LabelCoefficient = nullptr;
    QCheckBox *NVCheckBox = new QCheckBox("до настаящего времени");
    QPushButton *indexAddButton = new QPushButton("Добавить коэффициент");
    QPushButton *DefaultValueButton = new QPushButton("Востановить значения по умолчанию");
    QPushButton *SaveButton = new QPushButton("Сохранить");
    QGroupBox *groupBoxDebit = new QGroupBox(tr("Долг"));
    QRadioButton *lessRadioDebit = new QRadioButton(tr("Подсчитать в строну уменьшения долга. "
                                                       "В сторону уменьшение долга: гасятся в первую очередь ранние долги. "
                                                       "Дата погашения считаются относительно оплаты долгов."));
    QRadioButton *moreRadioDebit = new QRadioButton(tr("Подсчитать в строну увеличение долга. "
                                                       "В сторону увеличение долга: гасятся в первую очередь поздние долги. "
                                                       "Дата погашения считается относительно даты конца подсчётов."));
    QCheckBox *checkboxIgnoreString = nullptr;
    QCheckBox *checkboxConsiderPenny = nullptr;
    bool boolValueDebit = true;

public slots:
    void DefaultValue();
    //void TableBuilding(QString nameSetting);
    void AddButton();
    void SaveSettings(bool DialogClose = true);

private slots:
    void minimumValueChangedSlider(int min);
    void maximumValueChangedSlider(int max);
    void SBIndexValueChanged (int value);
    void ClickkedStateDebit();
    void monitorCheckStatus();
};

#endif // SETTINGSPENNY_H
