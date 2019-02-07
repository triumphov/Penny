#include "aboutprogramdialog.h"

AboutProgramDialog::AboutProgramDialog(QDialog *parent) :
    QDialog(parent)
{
    AboutProgramDialog::setWindowTitle(tr("О Программе"));
    AboutProgramDialog::setWindowIcon(QIcon(":/icon/penny.png"));
    AboutProgramDialog::setStyleSheet(" padding: 5px;");
    QString url = R"(:/icon/penny.png)";
    QPixmap img(url);
    QLabel *label = new QLabel(this);
    label->setPixmap(img);
    label->setFixedWidth(250);
    label->setFixedHeight(250);
    layout->addWidget(label,0,0);
    QLabel *LabelHead = new QLabel("<h1>Count pennies</h1>"
                                   "<h4>Авторские права:</h4>"
                                   "<span>Все права сохранены. Интеллектуальное права на идею, принадлежит юристу Дмитрию Зубкову.<br>"
                                   "Реализация идеи,  права на логотип \"Count pennies\" Иван Триумфов.</span>"
                                   "<h4>О программе:</h4>"
                                   "<p align=\"justify\">Быстрый способ подсчитать набежавшие пени. Программа гибкая, настраивается под любые<br>"
                                   "меняющиеся условия, есть импорт/экспорт в Excel. Ключевые ставке всегда актуальны,<br>"
                                   "потому что они в реальном времени беруться с интернет ресурсов. Обновления бесплатны.<br>"
                                   "Предназначена для юридических и управляющих компаний, которым надо взыскивать долги по<br>"
                                   "коммунальным платежам.</p>"
                                   "<h4>Версия программы: 1.0.0</h4>"
                                   "<h4>Собрано Дек 20 2018 в 11:00:10</h4>"
                                   "© Triumfov, 2018  Все права защищены.", this);
    layout->addWidget(LabelHead,0,1);
    this->setLayout(layout);
}

AboutProgramDialog::~AboutProgramDialog()
{

}
