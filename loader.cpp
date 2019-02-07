#include "loader.h"

#include <QDebug>
#include <QTextStream>
#include <QSslError>
#include <QRegExp>

Loader::Loader(QObject *parent) : QObject(parent)
{
    connect(m_manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(ReadRead(QNetworkReply*)));
}

void Loader::makeRequest(QString endPointRequest)
{
    m_manager->get(QNetworkRequest(QUrl(endPointRequest)));
}

void Loader::ReadRead(QNetworkReply *wikirep)
{
    QByteArray byteStr = wikirep->readAll();
    QString str = QString::fromUtf8(byteStr);
    qDebug() << "Response Received:\n" << str;

    QRegExp rxone("([\\d]{2}\\.[\\d]{2}\\.[\\d]{4})...([\\d]{2}\\.[\\d]{2}\\.[\\d]{4}).{4}([\\d,]{2,4})");
    QStringList listDataFrom;
    QStringList listDataTo;
    QStringList listRate;
    int posone = 0;
    while ((posone = rxone.indexIn(str, posone)) != -1) {
        listDataFrom << rxone.cap(1);
        listDataTo << rxone.cap(2);
        listRate << rxone.cap(3);
        posone += rxone.matchedLength();
    }

    QRegExp rxtwo("([\\d]{2}\\.[\\d]{2}\\.[\\d]{4})...(н.в.).{4}([\\d,]{2,4})");
    int postwo = 0;
    while ((postwo = rxtwo.indexIn(str, postwo)) != -1) {
        listDataFrom << rxtwo.cap(1);
        listDataTo << rxtwo.cap(2);
        listRate << rxtwo.cap(3);
        postwo += rxtwo.matchedLength();
    }

    wikirep->deleteLater();

    int row_arr_KeyRate = listRate.count();
    QString **arrKeyRate = new QString* [row_arr_KeyRate];
    int column_arr_KeyRate = 3;
    for (int i=0; i<row_arr_KeyRate;i++) {
        arrKeyRate[i] = new QString[column_arr_KeyRate];
    }

    for(int j=0; j<row_arr_KeyRate;j++) {
        arrKeyRate[j][0]=listDataFrom.at(j);
        arrKeyRate[j][1]=listDataTo.at(j);
        arrKeyRate[j][2]=listRate.at(j);
    }

    for(int j=0; j<row_arr_KeyRate;j++) {
        qDebug()<<"arrKeyRate"<<arrKeyRate[j][0]<<arrKeyRate[j][1]<<arrKeyRate[j][2]<<row_arr_KeyRate;
    }

    emit(dataReadyRead(arrKeyRate,row_arr_KeyRate));
}


