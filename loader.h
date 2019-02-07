#ifndef LOADER_H
#define LOADER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
class Loader : public QObject
{
    Q_OBJECT
public:
    explicit Loader(QObject *parent = nullptr);
    void makeRequest(QString endPointRequest);
signals:
    void dataReadyRead(QString **data,int row_arr_KeyRate);
public slots:
    void ReadRead(QNetworkReply *reply);
private:
    QNetworkAccessManager *m_manager =new QNetworkAccessManager(this);
};

#endif // LOADER_H
