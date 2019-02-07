#ifndef SETTINGSDATEFORMAT_H
#define SETTINGSDATEFORMAT_H

#include "settingsbase.h"

class SettingsDateFormat: public SettingsBase
{
    Q_OBJECT
public:
    explicit SettingsDateFormat(SettingsBase *SettB);
    ~SettingsDateFormat() { delete Settings; }
    void ApplySettings(QFileInfo info);

private:
    SettingsBase* Settings;
    QString LabelTextHead = "Пример выражения:dddd.MMMM.yyyy hhhh:mmmm:ssss\n"
                            "Допустимые значения между буквами: .,:;-_+ (пробел)/\\\n"
                            "Максимальное число символов:44";
    QString LabelTextHeadFormatDate = "Format (Формат) для типа данных \"Дата/время\"\n";
    QString LabelTextFormatDate ="d - День как число без нуля (от 1 до 31)\n"
                             "dd - День как число с нулём (от 01 до 31)\n"
                             "ddd - Сокращенное имя дня (например, «Пон» - «Вос»)\n"
                             "dddd - Длинное имя дня (например, «понедельник» - «воскресенье»)\n"
                             "M - Месяц как число без нуля (от 1 до 12)\n"
                             "MM - Месяц как число с нулём (от 01 до 12)\n"
                             "MMM - Сокращенное имя месяца (например, «Янв» - «Дек»)\n"
                             "MMMM - Длинное имя месяца (например, «январь» - «декабрь»)\n"
                             "yy - Год как двухзначное число (от 00 до 99)\n"
                             "yyyy - Год как четырехзначный номер.";
    QString LabelTextFormatTime ="d - День как число без нуля (от 1 до 31)\n"
                             "h - Час без двух нулей (от 0 до 23 или от 1 до 12)\n"
                             "hh - Час с двумя нулями (от 00 до 23 или от 01 до 12)\n"
                             "H - Час без двух нулей (от 0 до 23)\n"
                             "HH - Час с двумя нулями (от 00 до 23)\n"
                             "m - Минута без двух нулей (от 0 до 59)\n"
                             "mm - Минута с двумя нулями (от 00 до 59)\n"
                             "s - Секунда без двух нулей (от 0 до 59)\n"
                             "ss - Секунда с двумя нулями (от 00 до 59).";

    QLabel *LabelFormatHead = new QLabel(LabelTextHead, this);
    QLabel *LabelFormatHeadFormatDate = new QLabel(LabelTextHeadFormatDate, this);
    QLabel *LabelDateTextFormat = new QLabel(LabelTextFormatDate, this);
    QLabel *LabelTimeTextFormat = new QLabel(LabelTextFormatTime, this);
    QLineEdit *LineEdit = nullptr;
    QPushButton *dateFormatAddButton = nullptr;

public slots:
    void DefaultValue();
    //void TableBuilding(QString nameSetting);
    void AddButton();
    void SaveSettings(bool DialogClose = true);
};

#endif // SETTINGSDATEFORMAT_H
